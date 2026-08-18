#pragma once
#include "Order.h"
#include "Types.h"

struct PriceLevel {
    Price price = 0;
    Order* head = nullptr;
    Order* tail = nullptr;

    void pushBack(Order* order) {
        order->prev = tail;
        order->next = nullptr;

        // if list non-empty...
        if (tail != nullptr) {
            tail->next = order;
        }
        else {
            head = order;
        }

        tail = order;
    }

    void unlink(Order* order) {

		// if order isnt head...
        if (order->prev != nullptr) {
            order->prev->next = order->next;
        }
        else {
            head = order->next;
        }

        if (order->next != nullptr) {
            order->next->prev = order->prev;
        }
        else {
            tail = order->prev;
        }

        order->prev = nullptr;
        order->next = nullptr;
    }
};