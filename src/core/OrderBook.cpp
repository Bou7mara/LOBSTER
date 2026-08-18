#include "OrderBook.h"

void OrderBook::submit(Order order) {
    // 1. Allocate order inside std::deque (guarantees memory address stability)
    orders_.push_back(std::move(order));
    Order* storedOrder = &orders_.back();

    // 2. Insert into corresponding price level and map side
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

    // 3. Register handle in lookup map for O(1) cancel access
    orderLookup_[storedOrder->id] = OrderHandle{
        .price = storedOrder->price,
        .side = storedOrder->side,
        .order = storedOrder
    };
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