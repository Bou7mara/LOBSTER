#pragma once

#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Fill.h"
#include "Order.h"
#include "PriceLevel.h"
#include "Types.h"

namespace lobster {

struct OrderHandle {
    Price price;
    Side side;
    Order* order;
};

class OrderBook {
public:
    OrderBook() = default;

    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    OrderBook(OrderBook&&) = default;
    OrderBook& operator=(OrderBook&&) = default;

    std::vector<Fill> submit(Order order);
    bool cancel(OrderId id);

    [[nodiscard]] std::optional<Price> bestBid() const;
    [[nodiscard]] std::optional<Price> bestAsk() const;

private:
    bool pricesCross(const Order& incoming, Price levelPrice) const;
    std::vector<Fill> match(Order& incoming);

private:
    std::deque<Order> orders_;
    std::unordered_map<OrderId, OrderHandle> orderLookup_;
    std::map<Price, PriceLevel, std::greater<Price>> bids_;
    std::map<Price, PriceLevel, std::less<Price>> asks_;
};

}