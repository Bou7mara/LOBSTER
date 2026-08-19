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
