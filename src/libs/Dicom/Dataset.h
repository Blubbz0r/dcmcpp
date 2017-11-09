#pragma once

#include "DataElement.h"

namespace dcmcpp
{

// TODO: doc
class Dataset
{
public:
    explicit Dataset(std::vector<DataElement> dataElements);

    // TODO: doc
    const DataElement& dataElement(const Tag& tag) const;

private:
    std::vector<DataElement>::const_iterator findDataElementByTag(const Tag& tag) const;

    std::vector<DataElement> m_dataElements;
};

}
