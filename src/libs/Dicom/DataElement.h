#pragma once

#include "ApplicationEntity.h"
#include "CodeString.h"
#include "Date.h"
#include "DateTime.h"
#include "DecimalString.h"
#include "IntegerString.h"
#include "LongString.h"
#include "OtherByte.h"
#include "OtherWord.h"
#include "PersonName.h"
#include "Sequence.h"
#include "ShortString.h"
#include "ShortText.h"
#include "Tag.h"
#include "Time.h"
#include "UniqueIdentifier.h"
#include "UnlimitedText.h"
#include "UnsignedLong.h"
#include "UnsignedShort.h"
#include "ValueLength.h"
#include "ValueRepresentation.h"

#include <variant>

namespace dcmcpp
{

struct DataElement
{
    Tag tag;
    ValueRepresentation valueRepresentation;
    ExtendedValueLength valueLength; // TODO: this could be 16 or 32 bit... we hard-code the bigger type here

    using Value = std::variant<OtherByte, Sequence, UniqueIdentifier, UnsignedLong>; // TODO: is there a type trait that allows to declare all VRs in the variant even if they have a duplicated underlying type? we are currently missing Date, Time, CodeString, LongString, PersonName, ShortText, IntegerString, ShortString, DateTime, ApplicationEntity, UnlimitedText, DecimalString, UnsignedShort, OtherWord
    Value value;
};

std::string valueToString(const DataElement::Value& value);

// TODO: or maybe std::set since elements need to be unique?
using DataElements = std::vector<DataElement>;

const DataElement& dataElement(const DataElements& dataElements, const Tag& tag);
DataElements::const_iterator findDataElementByTag(const DataElements& dataElements,
                                                  const Tag& tag);

}
