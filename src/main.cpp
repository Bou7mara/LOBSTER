#include <iostream>
#include "core/Types.h"
#include "core/Order.h"
#include "core/PriceLevel.h"

void printLevel(const char* name, const PriceLevel& level) {
    std::cout << name << " (Price: " << level.price << "):\n";
    const Order* curr = level.head;
    if (!curr) {
        std::cout << "  [Empty]\n";
        return;
    }
    while (curr != nullptr) {
        std::cout << "  -> Order ID: " << curr->id
                  << " | Owner: " << curr->owner
                  << " | Qty: " << curr->quantity
                  << " | Side: " << (curr->side == Side::Bid ? "BID" : "ASK")
                  << " | SubmittedAt: " << curr->submittedAt << "\n";
        curr = curr->next;
    }
}

int main() {
    std::cout << "=== LOBSTER Limit Order Book - PriceLevel Demo ===\n\n";

    PriceLevel bidLevel;
    bidLevel.price = 15000;

    Order o1; o1.id = 101; o1.owner = 1; o1.side = Side::Bid; o1.price = 15000; o1.quantity = 50; o1.submittedAt = 1000;
    Order o2; o2.id = 102; o2.owner = 2; o2.side = Side::Bid; o2.price = 15000; o2.quantity = 30; o2.submittedAt = 1001;
    Order o3; o3.id = 103; o3.owner = 3; o3.side = Side::Bid; o3.price = 15000; o3.quantity = 20; o3.submittedAt = 1002;

    std::cout << "1. Enqueuing 3 orders into Bid PriceLevel...\n";
    bidLevel.pushBack(&o1);
    bidLevel.pushBack(&o2);
    bidLevel.pushBack(&o3);
    printLevel("Bid Level", bidLevel);

    std::cout << "\n2. Unlinking middle order (ID: 102)...\n";
    bidLevel.unlink(&o2);
    printLevel("Bid Level after unlinking ID 102", bidLevel);

    std::cout << "\n3. Unlinking head order (ID: 101)...\n";
    bidLevel.unlink(&o1);
    printLevel("Bid Level after unlinking ID 101", bidLevel);

    std::cout << "\n4. Unlinking remaining order (ID: 103)...\n";
    bidLevel.unlink(&o3);
    printLevel("Bid Level after unlinking ID 103", bidLevel);

    std::cout << "\nDemo complete.\n";
    return 0;
}