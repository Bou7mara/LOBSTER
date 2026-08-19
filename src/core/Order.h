#pragma once
#include "Types.h"

struct Order {
    OrderId id;
    AgentId owner;
    Side side;
    Price price;
    Quantity quantity;
    Tick submittedAt;

    Order* next = nullptr;
    Order* prev = nullptr;

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    Order(Order&&) = default;
    Order& operator=(Order&&) = default;

    Order() = default;
};

