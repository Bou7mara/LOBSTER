#pragma once
#include <cstdint>

namespace lobster {

using Price = int64_t;
using OrderId = uint64_t;
using Quantity = uint64_t;
using AgentId = uint64_t;
using OrderCount = uint32_t;

enum class Side {Bid, Ask};

}
