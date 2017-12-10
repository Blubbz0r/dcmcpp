#pragma once

#include "dcmcpp/Dicom/Tag.h"

#include <vector>

namespace dcmcpp
{

struct DataElement;

struct SequenceItem
{
    Tag tag;
    uint32_t valueLength;

    // TODO: we have a cycle here; SequenceItem::dataElements depends on DataElement while
    // DataElement::value depends on Sequence; not sure if this is really clean
    std::vector<DataElement> dataElements;
};

struct Sequence
{
    std::vector<SequenceItem> items;
};

}
