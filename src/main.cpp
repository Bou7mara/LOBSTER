#include <iostream>
#include "core/Types.h"
#include "core/Order.h"

int main() {
    Order buyOrder;
    buyOrder.id = 101;
    buyOrder.owner = 1;
    buyOrder.side = Side::Bid;
    buyOrder.price = 15000;
    buyOrder.quantity = 50;
    buyOrder.submittedAt = 1000;

    Order sellOrder;
    sellOrder.id = 102;
    sellOrder.owner = 2;
    sellOrder.side = Side::Ask;
    sellOrder.price = 15050;
    sellOrder.quantity = 25;
    sellOrder.submittedAt = 1001;

    std::cout << "Order test\n";
    std::cout << "Buy Order ID: " << buyOrder.id
              << " | Price: " << buyOrder.price
              << " | Quantity: " << buyOrder.quantity
              << " | Side: " << (buyOrder.side == Side::Ask ? "Ask" : "Bid") << "\n";

    std::cout << "Sell Order ID: " << sellOrder.id
              << " | Price: " << sellOrder.price
              << " | Qty: " << sellOrder.quantity
              << " | Side: " << (sellOrder.side == Side::Ask ? "ASK" : "BID") << "\n";

    if (buyOrder.prev == nullptr && buyOrder.next == nullptr) {
        std::cout << "Intrusive pointers default-initialized to nullptr correctly.\n";
    }

    return 0;
}