#include <catch2/catch_test_macros.hpp>
#include "core/OrderBook.h"

TEST_CASE("OrderBook - submit and bestBid/bestAsk queries", "[OrderBook]") {
    OrderBook book;

    REQUIRE_FALSE(book.bestBid().has_value());
    REQUIRE_FALSE(book.bestAsk().has_value());

    SECTION("submit bid orders updates bestBid correctly") {
        Order b1; b1.id = 1; b1.owner = 10; b1.side = Side::Bid; b1.price = 100; b1.quantity = 10; b1.submittedAt = 1;
        Order b2; b2.id = 2; b2.owner = 11; b2.side = Side::Bid; b2.price = 105; b2.quantity = 5; b2.submittedAt = 2;

        book.submit(std::move(b1));
        REQUIRE(book.bestBid() == 100);

        book.submit(std::move(b2));
        REQUIRE(book.bestBid() == 105);
    }

    SECTION("submit ask orders updates bestAsk correctly") {
        Order a1; a1.id = 3; a1.owner = 12; a1.side = Side::Ask; a1.price = 200; a1.quantity = 10; a1.submittedAt = 1;
        Order a2; a2.id = 4; a2.owner = 13; a2.side = Side::Ask; a2.price = 195; a2.quantity = 5; a2.submittedAt = 2;

        book.submit(std::move(a1));
        REQUIRE(book.bestAsk() == 200);

        book.submit(std::move(a2));
        REQUIRE(book.bestAsk() == 195);
    }
}

TEST_CASE("OrderBook - cancel operations", "[OrderBook]") {
    OrderBook book;

    Order b1; b1.id = 1; b1.owner = 10; b1.side = Side::Bid; b1.price = 100; b1.quantity = 10; b1.submittedAt = 1;
    Order b2; b2.id = 2; b2.owner = 11; b2.side = Side::Bid; b2.price = 105; b2.quantity = 5; b2.submittedAt = 2;

    book.submit(std::move(b1));
    book.submit(std::move(b2));

    SECTION("cancel non-existent order returns false") {
        REQUIRE_FALSE(book.cancel(999));
    }

    SECTION("cancel best bid updates bestBid query") {
        REQUIRE(book.cancel(2) == true);
        REQUIRE(book.bestBid() == 100);

        REQUIRE(book.cancel(1) == true);
        REQUIRE_FALSE(book.bestBid().has_value());
    }
}

TEST_CASE("OrderBook - Matching Engine", "[OrderBook]") {
    OrderBook book;

    SECTION("Exact full match") {
        Order ask; ask.id = 1; ask.owner = 10; ask.side = Side::Ask; ask.price = 100; ask.quantity = 10; ask.submittedAt = 1;
        book.submit(std::move(ask));

        Order bid; bid.id = 2; bid.owner = 20; bid.side = Side::Bid; bid.price = 100; bid.quantity = 10; bid.submittedAt = 2;
        auto fills = book.submit(std::move(bid));

        REQUIRE(fills.size() == 1);
        REQUIRE(fills[0].incomingId == 2);
        REQUIRE(fills[0].restingId == 1);
        REQUIRE(fills[0].price == 100);
        REQUIRE(fills[0].quantity == 10);

        REQUIRE_FALSE(book.bestAsk().has_value());
        REQUIRE_FALSE(book.bestBid().has_value());
    }

    SECTION("Partial match - incoming larger than resting") {
        Order ask; ask.id = 1; ask.owner = 10; ask.side = Side::Ask; ask.price = 100; ask.quantity = 10; ask.submittedAt = 1;
        book.submit(std::move(ask));

        Order bid; bid.id = 2; bid.owner = 20; bid.side = Side::Bid; bid.price = 105; bid.quantity = 25; bid.submittedAt = 2;
        auto fills = book.submit(std::move(bid));

        REQUIRE(fills.size() == 1);
        REQUIRE(fills[0].quantity == 10);

        REQUIRE_FALSE(book.bestAsk().has_value());
        REQUIRE(book.bestBid() == 105);
    }

    SECTION("Multi-level price-time priority sweep") {
        Order a1; a1.id = 1; a1.owner = 10; a1.side = Side::Ask; a1.price = 100; a1.quantity = 10; a1.submittedAt = 1;
        Order a2; a2.id = 2; a2.owner = 11; a2.side = Side::Ask; a2.price = 100; a2.quantity = 15; a2.submittedAt = 2;
        Order a3; a3.id = 3; a3.owner = 12; a3.side = Side::Ask; a3.price = 102; a3.quantity = 20; a3.submittedAt = 3;

        book.submit(std::move(a1));
        book.submit(std::move(a2));
        book.submit(std::move(a3));

        Order bigBid; bigBid.id = 10; bigBid.owner = 30; bigBid.side = Side::Bid; bigBid.price = 105; bigBid.quantity = 30; bigBid.submittedAt = 4;
        auto fills = book.submit(std::move(bigBid));

        REQUIRE(fills.size() == 3);
        // First fill against a1 at 100 (qty 10)
        REQUIRE(fills[0].restingId == 1);
        REQUIRE(fills[0].quantity == 10);

        // Second fill against a2 at 100 (qty 15)
        REQUIRE(fills[1].restingId == 2);
        REQUIRE(fills[1].quantity == 15);

        // Third fill against a3 at 102 (qty 5)
        REQUIRE(fills[2].restingId == 3);
        REQUIRE(fills[2].quantity == 5);

        REQUIRE(book.bestAsk() == 102);
        REQUIRE_FALSE(book.bestBid().has_value());
    }
}
