#pragma once

#include <cstdint>

namespace dcmcpp
{

using ValueLength = uint16_t;
using ExtendedValueLength = uint32_t;

static constexpr uint32_t UndefinedValueLength = 0xffffffff;

}
