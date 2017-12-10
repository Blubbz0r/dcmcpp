#pragma once

#include <cstdint>
#include <string_view>

namespace dcmcpp
{

enum class ValueRepresentation;

using ValueLength = uint16_t;
using ExtendedValueLength = uint32_t;

static constexpr uint32_t UndefinedValueLength = 0xffffffff;

// value length can either be 2 or 4 byte as defined in PS3.5 chapter "Data elements structure with Explicit/Implicit VR"
bool hasExtendedLengthEncoding(ValueRepresentation vr, std::string_view transferSyntaxUid);

}
