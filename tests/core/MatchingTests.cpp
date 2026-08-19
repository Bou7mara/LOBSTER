#include <catch2/catch_test_macros.hpp>
#include "core/OrderBook.h"

using namespace lobster;

TEST_CASE("Matching: exact-size fill") {
    OrderBook book;
    book.submit(Order{1, 1, Side::Ask, 100, 10, 0});
    auto fills = book.submit(Order{2, 1, Side::Bid, 100, 10, 1});

    REQUIRE(fills.size() == 1);
    REQUIRE(fills[0].quantity == 10);
    REQUIRE(book.bestAsk() == std::nullopt);  // resting side fully consumed
}

TEST_CASE("Matching: partial fill") {
    OrderBook book;
    book.submit(Order{1, 1, Side::Ask, 100, 20, 0});
    auto fills = book.submit(Order{2, 1, Side::Bid, 100, 5, 1});

    REQUIRE(fills.size() == 1);
    REQUIRE(fills[0].quantity == 5);
    REQUIRE(book.bestAsk() == 100);  // resting order still there, reduced qty
}

TEST_CASE("Matching: multi-level sweep") {
    OrderBook book;
    book.submit(Order{1, 1, Side::Ask, 100, 5, 0});
    book.submit(Order{2, 1, Side::Ask, 101, 5, 0});
    auto fills = book.submit(Order{3, 1, Side::Bid, 101, 10, 1});

    REQUIRE(fills.size() == 2);
    REQUIRE(book.bestAsk() == std::nullopt);
}

TEST_CASE("Matching: price-time priority at the same price") {
    OrderBook book;
    book.submit(Order{1, 1, Side::Ask, 100, 10, 0});  // older
    book.submit(Order{2, 1, Side::Ask, 100, 10, 1});  // newer
    auto fills = book.submit(Order{3, 1, Side::Bid, 100, 10, 2});

    REQUIRE(fills.size() == 1);
    REQUIRE(fills[0].restingId == 1);  // older order fills first
}

TEST_CASE("Matching: non-crossing order still rests") {
    OrderBook book;
    book.submit(Order{1, 1, Side::Ask, 110, 10, 0});
    auto fills = book.submit(Order{2, 1, Side::Bid, 100, 10, 1});

    REQUIRE(fills.empty());
    REQUIRE(book.bestBid() == 100);
}

TEST_CASE("Matching: incoming exactly exhausts the opposite side, nothing left over") {
    OrderBook book;
    book.submit(Order{1, 1, Side::Ask, 100, 10, 0});
    auto fills = book.submit(Order{2, 1, Side::Bid, 100, 10, 1});

    REQUIRE(fills.size() == 1);
    REQUIRE(book.bestBid() == std::nullopt);  // incoming had nothing left to rest
}
