#include "DataElement.h"

#include <algorithm>

namespace dcmcpp
{

template<class T> struct always_false : std::false_type {};

std::string valueToString(const DataElement::Value& value)
{
   return std::visit(
       [](auto&& arg) {
          using T= std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, OtherByte>)
             return std::to_string(arg);
          else if constexpr (std::is_same_v<T, UniqueIdentifier>)
             return arg;
          else if constexpr (std::is_same_v<T, UnsignedLong>)
             return std::to_string(arg);
          else if constexpr (std::is_same_v<T, Sequence>)
            // TODO: a Sequence cannot hold a value directly (?) thus, we return an empty string here
            // user needs to recurse through the data elements in a sequence to get the individual values
             return std::string();
          else
             static_assert(always_false<T>::value, "non-exhaustive visitor!");
       },
       value);
}

const DataElement& dataElement(const DataElements& dataElements, const Tag& tag)
{
    const auto it = findDataElementByTag(dataElements, tag);
    if (it != dataElements.cend())
        return *it;

    // TODO: proper exception
    throw std::logic_error("No data element for tag '" + tagToString(tag) + "'");
}

DataElements::const_iterator findDataElementByTag(const DataElements& dataElements,
                                                  const Tag& tag)
{
   return std::find_if(dataElements.cbegin(), dataElements.cend(),
                       [&tag](const DataElement& element) { return element.tag == tag; });
}

}
