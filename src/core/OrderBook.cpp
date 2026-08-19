#include "OrderBook.h"
#include <algorithm>

void OrderBook::match(Order& incoming, std::vector<Fill>& fills) {
    if (incoming.side == Side::Bid) {
        while (!asks_.empty() && asks_.begin()->first <= incoming.price && incoming.quantity > 0) {
            auto askIt = asks_.begin();
            PriceLevel& level = askIt->second;
            Order* resting = level.head;

            while (resting != nullptr && incoming.quantity > 0) {
                Order* nextResting = resting->next;
                Quantity fillQty = std::min(incoming.quantity, resting->quantity);

                incoming.quantity -= fillQty;
                resting->quantity -= fillQty;

                fills.push_back(Fill{
                    .incomingId = incoming.id,
                    .restingId = resting->id,
                    .price = resting->price,
                    .quantity = fillQty
                });

                if (resting->quantity == 0) {
                    level.unlink(resting);
                    orderLookup_.erase(resting->id);
                }

                resting = nextResting;
            }

            if (level.head == nullptr) {
                asks_.erase(askIt);
            }
        }
    }
    else {
        while (!bids_.empty() && bids_.begin()->first >= incoming.price && incoming.quantity > 0) {
            auto bidIt = bids_.begin();
            PriceLevel& level = bidIt->second;
            Order* resting = level.head;

            while (resting != nullptr && incoming.quantity > 0) {
                Order* nextResting = resting->next;
                Quantity fillQty = std::min(incoming.quantity, resting->quantity);

                incoming.quantity -= fillQty;
                resting->quantity -= fillQty;

                fills.push_back(Fill{
                    .incomingId = incoming.id,
                    .restingId = resting->id,
                    .price = resting->price,
                    .quantity = fillQty
                });

                if (resting->quantity == 0) {
                    level.unlink(resting);
                    orderLookup_.erase(resting->id);
                }

                resting = nextResting;
            }

            if (level.head == nullptr) {
                bids_.erase(bidIt);
            }
        }
    }
}

std::vector<Fill> OrderBook::submit(Order order) {
    std::vector<Fill> fills;

    // 1. Match incoming order against resting orders on opposite side
    match(order, fills);

    // 2. If leftover quantity remains, insert into book as resting order
    if (order.quantity > 0) {
        orders_.push_back(std::move(order));
        Order* storedOrder = &orders_.back();

        if (storedOrder->side == Side::Bid) {
            auto& level = bids_[storedOrder->price];
            level.price = storedOrder->price;
            level.pushBack(storedOrder);
        }
        else {
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
        return false; // Fails gracefully if OrderId is not present
    }

    const OrderHandle handle = it->second;

    // 1. Unlink order from its price level and cleanup empty price levels
    if (handle.side == Side::Bid) {
        auto levelIt = bids_.find(handle.price);
        if (levelIt != bids_.end()) {
            levelIt->second.unlink(handle.order);
            if (levelIt->second.head == nullptr) {
                bids_.erase(levelIt);
            }
        }
    }
    else {
        auto levelIt = asks_.find(handle.price);
        if (levelIt != asks_.end()) {
            levelIt->second.unlink(handle.order);
            if (levelIt->second.head == nullptr) {
                asks_.erase(levelIt);
            }
        }
    }

    // 2. Remove handle from lookup map
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