#pragma once

#include "DataElement.h"

namespace dcmcpp
{

// TODO: doc
class Dataset
{
public:
    explicit Dataset(DataElements dataElements);

    // TODO: doc
    // TODO: duplicated in FileMetaInfo; management of a list of DataElements should probably move to a separate class
    const DataElements& dataElements() const;

private:
    DataElements m_dataElements;
};

}
