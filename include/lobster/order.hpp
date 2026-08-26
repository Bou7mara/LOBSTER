#pragma once

#include "types.hpp"

namespace lobster {

struct Order {
    OrderId id = 0;
    AgentId owner = 0;
    Side side = Side::Bid;
    Price price = 0;
    Quantity quantity = 0;
    Tick submittedAt = 0;

    Order* next = nullptr;
    Order* prev = nullptr;

    Order() = default;
    Order(OrderId id_, AgentId owner_, Side side_, Price price_, Quantity quantity_, Tick submittedAt_ = 0)
        : id(id_), owner(owner_), side(side_), price(price_), quantity(quantity_), submittedAt(submittedAt_) {}

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    Order(Order&&) = default;
    Order& operator=(Order&&) = default;
};

}
