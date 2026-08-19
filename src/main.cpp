#include <iostream>
#include "core/Types.h"
#include "core/Order.h"
#include "core/OrderBook.h"

int main() {
    std::cout << "=== LOBSTER Limit Order Book - OrderBook Demo ===\n\n";

    OrderBook book;

    std::cout << "1. Submitting Bid Orders...\n";
    Order b1; b1.id = 101; b1.owner = 1; b1.side = Side::Bid; b1.price = 15000; b1.quantity = 50; b1.submittedAt = 1000;
    Order b2; b2.id = 102; b2.owner = 2; b2.side = Side::Bid; b2.price = 15050; b2.quantity = 30; b2.submittedAt = 1001;

    book.submit(std::move(b1));
    book.submit(std::move(b2));

    if (auto bestBid = book.bestBid()) {
        std::cout << "  Current Best Bid: " << *bestBid << "\n";
    }

    std::cout << "\n2. Submitting Ask Orders...\n";
    Order a1; a1.id = 201; a1.owner = 3; a1.side = Side::Ask; a1.price = 15100; a1.quantity = 25; a1.submittedAt = 1002;
    Order a2; a2.id = 202; a2.owner = 4; a2.side = Side::Ask; a2.price = 15080; a2.quantity = 40; a2.submittedAt = 1003;

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