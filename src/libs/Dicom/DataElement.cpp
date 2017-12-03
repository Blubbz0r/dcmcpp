#include "DataElement.h"

#include <algorithm>
#include <sstream>

namespace dcmcpp
{

template<class T> struct always_false : std::false_type {};

DataElement::DataElement() : valueRepresentation(ValueRepresentation::None), valueLength(0)
{
}

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

std::string dumpDataElements(const DataElements& dataElements, int indentationLevel)
{
    std::stringstream stream;
    std::string indentation;
    for (int i = 0; i < indentationLevel; ++i)
        indentation += "  ";

    for (const auto& dataElement : dataElements)
    {
        stream << indentation << tagToString(dataElement.tag) << '\t';
        stream << indentation << vrToString(dataElement.valueRepresentation) << '\t';
        // TODO: value length can have different digits possibly requiring multiple \t...
        stream << indentation << std::to_string(dataElement.valueLength) << '\t';
        stream << indentation << valueToString(dataElement.value) << "\n";

        if (dataElement.valueRepresentation == ValueRepresentation::SQ)
        {
            const auto& sequence = std::get<Sequence>(dataElement.value);
            for (const auto& sequenceItem : sequence.items)
                stream << dumpDataElements(sequenceItem.dataElements, indentationLevel + 1);
        }
    }

    return stream.str();
}

}
