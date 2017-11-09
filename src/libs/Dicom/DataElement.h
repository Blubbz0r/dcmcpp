#pragma once

#include "CodeString.h"
#include "Date.h"
#include "LongString.h"
#include "OtherByte.h"
#include "PersonName.h"
#include "ShortString.h"
#include "Tag.h"
#include "Time.h"
#include "UniqueIdentifier.h"
#include "UnsignedLong.h"
#include "ValueRepresentation.h"

#include <variant>

namespace dcmcpp
{

struct DataElement
{
    Tag tag;
    ValueRepresentation valueRepresentation;
    int valueLength; // TODO: it int appropriate? could be uint16_t or uint32_t (extended length)?
    std::variant<OtherByte, UniqueIdentifier, UnsignedLong> value; // TODO: is there a type trait that allows to declare all VRs in the variant even if they have a duplicated underlying type? we are currently missing Date, Time, CodeString, LongString, PersonName and ShortString
};

}
