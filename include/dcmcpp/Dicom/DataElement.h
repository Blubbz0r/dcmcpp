#pragma once

#include "dcmcpp/Dicom/ApplicationEntity.h"
#include "dcmcpp/Dicom/CodeString.h"
#include "dcmcpp/Dicom/Date.h"
#include "dcmcpp/Dicom/DateTime.h"
#include "dcmcpp/Dicom/DecimalString.h"
#include "dcmcpp/Dicom/IntegerString.h"
#include "dcmcpp/Dicom/LongString.h"
#include "dcmcpp/Dicom/OtherByte.h"
#include "dcmcpp/Dicom/OtherWord.h"
#include "dcmcpp/Dicom/PersonName.h"
#include "dcmcpp/Dicom/Sequence.h"
#include "dcmcpp/Dicom/ShortString.h"
#include "dcmcpp/Dicom/ShortText.h"
#include "dcmcpp/Dicom/Tag.h"
#include "dcmcpp/Dicom/Time.h"
#include "dcmcpp/Dicom/UniqueIdentifier.h"
#include "dcmcpp/Dicom/UnlimitedText.h"
#include "dcmcpp/Dicom/UnsignedLong.h"
#include "dcmcpp/Dicom/UnsignedShort.h"
#include "dcmcpp/Dicom/ValueLength.h"
#include "dcmcpp/Dicom/ValueRepresentation.h"

#include <variant>

namespace dcmcpp
{

struct DataElement
{
    DataElement();

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

// TODO: doc
std::string dumpDataElements(const DataElements& dataElements, int indentationLevel = 0);

}
