#include <iostream>
#include "lobster/types.hpp"
#include "lobster/order.hpp"
#include "lobster/priceLevel.hpp"

int main() {
    lobster::PriceLevel queue;
    queue.price = 10050;

    lobster::Order o1{1, 101, lobster::Side::Bid, 10050, 50, 1};
    lobster::Order o2{2, 102, lobster::Side::Bid, 10050, 30, 2};

    queue.pushBack(&o1);
    queue.pushBack(&o2);

    std::cout << "Price Level: " << queue.price << "\n";
    std::cout << "Total Volume: " << queue.totalVolume << " across " << queue.orderCount << " orders.\n";
    std::cout << "First in queue (Head ID): " << queue.head->id << " with qty " << queue.head->quantity << "\n";

    queue.unlink(&o1);
    std::cout << "After removing first order, new Head ID: " << queue.head->id
              << " | Remaining Volume: " << queue.totalVolume << "\n";

    return 0;
}
