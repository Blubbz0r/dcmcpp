#pragma once

#include "LittleEndian.h"
#include "StreamUtils.h"

#include <istream>
#include <string>
#include <type_traits>

namespace dcmcpp::LittleEndian
{

/*!
    Reads as many characters from the given \a stream as required for the given return type \a Ret.
*/
template<typename Ret>
Ret readIntegral(std::istream& stream)
{
    // TODO: or would SFINAE be better?
    static_assert(std::is_integral<Ret>::value, "Ret needs to be integral");

    static constexpr auto Size = sizeof(Ret);

    const auto availableBytes = StreamUtils::availableBytes(stream);
    if (availableBytes < Size)
    {
        // TODO: proper exception
        throw std::runtime_error(
            "Not enough bytes in stream to conver to the requested type (available: " +
            std::to_string(availableBytes) + "; required: " + std::to_string(Size) + ")");
    }

    char value[Size];
    stream.read(value, Size);
    return toIntegral<Ret>(value);
}

}
