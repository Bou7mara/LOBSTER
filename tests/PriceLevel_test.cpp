#include <catch2/catch_test_macros.hpp>
#include "core/Types.h"
#include "core/Order.h"
#include "core/PriceLevel.h"

using namespace lobster;

TEST_CASE("PriceLevel - pushBack operations", "[PriceLevel]") {
    PriceLevel level;
    level.price = 10000;

    Order o1; o1.id = 1; o1.owner = 10; o1.side = Side::Bid; o1.price = 10000; o1.quantity = 5; o1.submittedAt = 100;
    Order o2; o2.id = 2; o2.owner = 11; o2.side = Side::Bid; o2.price = 10000; o2.quantity = 10; o2.submittedAt = 101;

    SECTION("pushBack single order") {
        level.pushBack(&o1);

        REQUIRE(level.head == &o1);
        REQUIRE(level.tail == &o1);
        REQUIRE(o1.prev == nullptr);
        REQUIRE(o1.next == nullptr);
    }

    SECTION("pushBack multiple orders preserves FIFO links") {
        level.pushBack(&o1);
        level.pushBack(&o2);

        REQUIRE(level.head == &o1);
        REQUIRE(level.tail == &o2);

        // Forward links
        REQUIRE(o1.next == &o2);
        REQUIRE(o2.next == nullptr);

        // Backward links
        REQUIRE(o2.prev == &o1);
        REQUIRE(o1.prev == nullptr);
    }
}

TEST_CASE("PriceLevel - unlink edge cases", "[PriceLevel]") {
    PriceLevel level;
    level.price = 10000;

    Order o1; o1.id = 1; o1.owner = 10; o1.side = Side::Bid; o1.price = 10000; o1.quantity = 5; o1.submittedAt = 100;
    Order o2; o2.id = 2; o2.owner = 11; o2.side = Side::Bid; o2.price = 10000; o2.quantity = 10; o2.submittedAt = 101;
    Order o3; o3.id = 3; o3.owner = 12; o3.side = Side::Bid; o3.price = 10000; o3.quantity = 15; o3.submittedAt = 102;

    SECTION("unlink only element in list") {
        level.pushBack(&o1);
        level.unlink(&o1);

        REQUIRE(level.head == nullptr);
        REQUIRE(level.tail == nullptr);
        REQUIRE(o1.prev == nullptr);
        REQUIRE(o1.next == nullptr);
    }

    SECTION("unlink middle element of three") {
        level.pushBack(&o1);
        level.pushBack(&o2);
        level.pushBack(&o3);

        level.unlink(&o2);

        // Head and tail should remain unchanged
        REQUIRE(level.head == &o1);
        REQUIRE(level.tail == &o3);

        // Surrounding nodes link to each other
        REQUIRE(o1.next == &o3);
        REQUIRE(o3.prev == &o1);

        // Unlinked node pointers are cleared
        REQUIRE(o2.prev == nullptr);
        REQUIRE(o2.next == nullptr);
    }

    SECTION("unlink head element of three") {
        level.pushBack(&o1);
        level.pushBack(&o2);
        level.pushBack(&o3);

        level.unlink(&o1);

        REQUIRE(level.head == &o2);
        REQUIRE(level.tail == &o3);
        REQUIRE(o2.prev == nullptr);
        REQUIRE(o1.next == nullptr);
    }

    SECTION("unlink tail element of three") {
        level.pushBack(&o1);
        level.pushBack(&o2);
        level.pushBack(&o3);

        level.unlink(&o3);

        REQUIRE(level.head == &o1);
        REQUIRE(level.tail == &o2);
        REQUIRE(o2.next == nullptr);
        REQUIRE(o3.prev == nullptr);
    }
}