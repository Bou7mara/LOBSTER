#pragma once
#incldue "order.hpp"
#include "types.hpp"

namespace lobster {

struct PriceLevel {
    Price price = 0;
    Quantity totalVolume = 0;
    uint32_t orderCount = 0;
    Order* head = nullptr;
    Order* tail = nullptr;

    bool empty() const noexcept {
        return head == nullptr;
    }

    void pushBack(Order * order) noexcept {
        order->prev = tail;
        order->next = nullptr;

        if (tail != nullptr) {
            tail->next = order;
        }
        else {
            head = order;
        }

        tail = order;
        totalVolume += order->quantity;
        ++orderCount;
    }

    void unlink(Order* order) noexcept {
        if (order->prev != nullptr) {
            order->prev->next = order->next;
        } else {
            head = order->next;
        }

        if (order->next != nullptr) {
            order->next->prev = order->prev;
        } else {
            tail = order->prev;
        }

        totalVolume -= order->quantity;
        --orderCount;

        order->prev = nullptr;
        order->next = nullptr;
    }
};
}

