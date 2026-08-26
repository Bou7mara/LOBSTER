#pragma once
#include "Types.hpp"

namespace lobster {

Struct Order {
    OrderId id = 0;
    AgentId owner = 0;
    Side side = Side::Bid;
    Price price = 0;
    Quantity quantity = 0;
    Tick  submittedAt = 0;

    Order* next = nullptr;
    Order* prev = nullptr;

    Order* next = nullptr;
    Order* prev = nullptr;

    Order() = default;
     Order(OrderId id, AgentId owner, Side side, Price price, Quantity quantity, Tick submittedAt = 0)
        : id(id), owner(owner), side(side), price(price), quantity(quantity), submittedAt(submittedAt) {}

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete; 
    
    Order(Order&&) default;
    Order& operator=(Order&&) = default;
};
}

