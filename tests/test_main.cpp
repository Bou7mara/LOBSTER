#include <catch2/catch_test_macros.hpp>
#include "core/Order.h"

TEST_CASE("Order initialization", "[core]") {
    Order order;
    REQUIRE(order.next == nullptr);
    REQUIRE(order.prev == nullptr);
}