#include "DataElement.h"

namespace dcmcpp
{

template<class T> struct always_false : std::false_type {};

std::string valueToString(const DataElement::Value& value)
{
    return std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, OtherByte>)
            return std::to_string(arg);
        else if constexpr (std::is_same_v<T, UniqueIdentifier>)
            return arg;
        else if constexpr (std::is_same_v<T, UnsignedLong>)
            return std::to_string(arg);
        else
            static_assert(always_false<T>::value, "non-exhaustive visitor!");
    },
        value);
}

}
