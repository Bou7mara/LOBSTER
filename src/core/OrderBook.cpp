#include "OrderBook.h"
#include <algorithm>

namespace lobster {

bool OrderBook::pricesCross(const Order& incoming, Price levelPrice) const {
    if (incoming.side == Side::Bid) {
        return incoming.price >= levelPrice;
    } else {
        return incoming.price <= levelPrice;
    }
}

std::vector<Fill> OrderBook::match(Order& incoming) {
    std::vector<Fill> fills;

    if (incoming.side == Side::Bid) {
        while (incoming.quantity > 0 && !asks_.empty()) {
            auto levelIt = asks_.begin();
            PriceLevel& level = levelIt->second;
            if (!pricesCross(incoming, level.price)) {
                break;
            }

            while (incoming.quantity > 0 && level.head != nullptr) {
                Order* resting = level.head;
                Quantity tradeQty = std::min(incoming.quantity, resting->quantity);

                fills.push_back(Fill{
                    .incomingId = incoming.id,
                    .restingId = resting->id,
                    .price = level.price,
                    .quantity = tradeQty
                });

                incoming.quantity -= tradeQty;
                resting->quantity -= tradeQty;

                if (resting->quantity == 0) {
                    level.unlink(resting);
                    orderLookup_.erase(resting->id);
                }
            }

            if (level.head == nullptr) {
                asks_.erase(levelIt);
            }
        }
    } else {
        while (incoming.quantity > 0 && !bids_.empty()) {
            auto levelIt = bids_.begin();
            PriceLevel& level = levelIt->second;
            if (!pricesCross(incoming, level.price)) {
                break;
            }

            while (incoming.quantity > 0 && level.head != nullptr) {
                Order* resting = level.head;
                Quantity tradeQty = std::min(incoming.quantity, resting->quantity);

                fills.push_back(Fill{
                    .incomingId = incoming.id,
                    .restingId = resting->id,
                    .price = level.price,
                    .quantity = tradeQty
                });

                incoming.quantity -= tradeQty;
                resting->quantity -= tradeQty;

                if (resting->quantity == 0) {
                    level.unlink(resting);
                    orderLookup_.erase(resting->id);
                }
            }

            if (level.head == nullptr) {
                bids_.erase(levelIt);
            }
        }
    }

    return fills;
}

std::vector<Fill> OrderBook::submit(Order order) {
    std::vector<Fill> fills = match(order);

    if (order.quantity > 0) {
        orders_.push_back(std::move(order));
        Order* storedOrder = &orders_.back();

        if (storedOrder->side == Side::Bid) {
            auto& level = bids_[storedOrder->price];
            level.price = storedOrder->price;
            level.pushBack(storedOrder);
        } else {
            auto& level = asks_[storedOrder->price];
            level.price = storedOrder->price;
            level.pushBack(storedOrder);
        }

        orderLookup_[storedOrder->id] = OrderHandle{
            .price = storedOrder->price,
            .side = storedOrder->side,
            .order = storedOrder
        };
    }

    return fills;
}

bool OrderBook::cancel(OrderId id) {
    auto it = orderLookup_.find(id);
    if (it == orderLookup_.end()) {
        return false;
    }

    const OrderHandle handle = it->second;

    if (handle.side == Side::Bid) {
        auto levelIt = bids_.find(handle.price);
        if (levelIt != bids_.end()) {
            levelIt->second.unlink(handle.order);
            if (levelIt->second.head == nullptr) {
                bids_.erase(levelIt);
            }
        }
    } else {
        auto levelIt = asks_.find(handle.price);
        if (levelIt != asks_.end()) {
            levelIt->second.unlink(handle.order);
            if (levelIt->second.head == nullptr) {
                asks_.erase(levelIt);
            }
        }
    }

    orderLookup_.erase(it);
    return true;
}

std::optional<Price> OrderBook::bestBid() const {
    if (bids_.empty()) {
        return std::nullopt;
    }
    return bids_.begin()->first;
}

std::optional<Price> OrderBook::bestAsk() const {
    if (asks_.empty()) {
        return std::nullopt;
    }
    return asks_.begin()->first;
}

} // namespace lobster