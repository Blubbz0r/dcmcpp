#include "dcmcpp/Dicom/Dataset.h"

#include <algorithm>

namespace dcmcpp
{

Dataset::Dataset(std::vector<DataElement> dataElements)
    : m_dataElements(std::move(dataElements))
{
}

const DataElements& Dataset::dataElements() const
{
    return m_dataElements;
}

}
