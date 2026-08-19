#include <iostream>
#include "core/Types.h"
#include "core/Order.h"
#include "core/OrderBook.h"

using namespace lobster;

int main() {
    std::cout << "=== LOBSTER Limit Order Book - OrderBook Demo ===\n\n";

    OrderBook book;

    std::cout << "1. Submitting Bid Orders...\n";
    Order b1{101, 1, Side::Bid, 15000, 50, 1000};
    Order b2{102, 2, Side::Bid, 15050, 30, 1001};

    book.submit(std::move(b1));
    book.submit(std::move(b2));

    if (auto bestBid = book.bestBid()) {
        std::cout << "  Current Best Bid: " << *bestBid << "\n";
    }

    std::cout << "\n2. Submitting Ask Orders...\n";
    Order a1{201, 3, Side::Ask, 15100, 25, 1002};
    Order a2{202, 4, Side::Ask, 15080, 40, 1003};

    book.submit(std::move(a1));
    book.submit(std::move(a2));

    if (auto bestAsk = book.bestAsk()) {
        std::cout << "  Current Best Ask: " << *bestAsk << "\n";
    }

    std::cout << "\n3. Canceling Best Bid (ID: 102 @ 15050)...\n";
    if (book.cancel(102)) {
        std::cout << "  Order 102 canceled successfully.\n";
    }
    if (auto bestBid = book.bestBid()) {
        std::cout << "  New Best Bid: " << *bestBid << "\n";
    }

    std::cout << "\n4. Canceling Best Ask (ID: 202 @ 15080)...\n";
    if (book.cancel(202)) {
        std::cout << "  Order 202 canceled successfully.\n";
    }
    if (auto bestAsk = book.bestAsk()) {
        std::cout << "  New Best Ask: " << *bestAsk << "\n";
    }

    std::cout << "\nDemo complete.\n";
    return 0;
}