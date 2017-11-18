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
    // TODO: duplicated in FileMetaInfo; management of a list of DataElements should probably move to a separate class
    const DataElement& dataElement(const Tag& tag) const;
    const std::vector<DataElement>& dataElements() const;

private:
    std::vector<DataElement>::const_iterator findDataElementByTag(const Tag& tag) const;

    std::vector<DataElement> m_dataElements;
};

}
