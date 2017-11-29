#pragma once

#include <cstddef>
#include <type_traits>

#include <gsl/span>

/*! Functions to convert Little Endian encoded data. */
namespace dcmcpp::LittleEndian
{

/*!
    Converts the given character array \s to the given integral type \a Ret.
    Uses the first n bytes of the array \s based on the size of the return type.

    Usage example:
    \code
    constexpr char input[2] = { '\x4', '\x2' };
    constexpr auto value = toInt<uint16_t>(input); // value == 516

    constexpr char input2[4] = { '\x2', '\0', '\x4', '\0' };
    constexpr auto value2 = toInt<uint32_t>(input); // value == 262146
    \endcode
*/
template <typename Ret, std::size_t N>
constexpr Ret toIntegral(const char (&s)[N])
{
    // TODO: or would SFINAE be better?
    static_assert(std::is_integral<Ret>::value, "Ret needs to be integral");
    constexpr auto size = sizeof(Ret);
    static_assert(N >= size, "Array not big enough to convert to the requested type");
    Ret ret = 0;
    for (int i = size - 1; i >= 0; --i)
        ret |= (s[i] & 0xff) << (8 * i);
    return ret;
}

// TODO: same as above for runtime; can we avoid the duplication?
template<typename Ret, typename DataType>
Ret toIntegral(gsl::span<DataType> data)
{
    constexpr auto size = sizeof(Ret);
    if (data.size() < size)
        throw std::logic_error("Not enough bytes in data to convert to the requested type"); // TODO: proper exception?

    Ret ret = 0;
    for (int i = size - 1; i >= 0; --i)
        ret |= (data[i] & 0xff) << (8 * i);
    return ret;
}

}
