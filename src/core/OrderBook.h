#pragma once

#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <unordered_map>

#include "Order.h"
#include "PriceLevel.h"
#include "Types.h"

// Handle storing metadata required for O(1) unlinking and removal
struct OrderHandle {
    Price price;
    Side side;
    Order* order;
};

class OrderBook {
public:
    OrderBook() = default;

    // Prevent copying to safeguard internal Order pointer integrity
    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    // Default move operations
    OrderBook(OrderBook&&) = default;
    OrderBook& operator=(OrderBook&&) = default;

    // Core Operations
    void submit(Order order);
    bool cancel(OrderId id);

    // Queries
    [[nodiscard]] std::optional<Price> bestBid() const;
    [[nodiscard]] std::optional<Price> bestAsk() const;

private:
    // Memory Storage: std::deque guarantees pointer stability upon push_back/emplace_back
    std::deque<Order> orders_;

    // Order lookup map for O(1) cancels
    std::unordered_map<OrderId, OrderHandle> orderLookup_;

    // Bid Side: Sorted high-to-low (std::greater) so best bid is at begin()
    std::map<Price, PriceLevel, std::greater<Price>> bids_;

    // Ask Side: Sorted low-to-high (std::less) so best ask is at begin()
    std::map<Price, PriceLevel, std::less<Price>> asks_;
};