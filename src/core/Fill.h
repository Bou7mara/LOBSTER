#pragma once

#include "Types.h"

struct Fill {
    OrderId incomingId;
    OrderId restingId;
    Price price;
    Quantity quantity;

    bool operator==(const Fill&) const = default;
};
