#include "Dataset.h"

#include <algorithm>

namespace dcmcpp
{

Dataset::Dataset(std::vector<DataElement> dataElements)
    : m_dataElements(std::move(dataElements))
{
}

const DataElement& Dataset::dataElement(const Tag& tag) const
{
    const auto it = findDataElementByTag(tag);
    if (it != m_dataElements.cend())
        return *it;

    // TODO: proper exception
    throw std::logic_error("No data element for tag '" + tagToString(tag) + "'");
}

const std::vector<DataElement>& Dataset::dataElements() const
{
    return m_dataElements;
}

std::vector<DataElement>::const_iterator Dataset::findDataElementByTag(const Tag& tag) const
{
    return std::find_if(m_dataElements.cbegin(), m_dataElements.cend(), [&tag](const DataElement& element)
    {
        return element.tag == tag;
    });
}

}
