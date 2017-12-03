#include "FileMetaInfo.h"
#include "Dictionary.h"

#include <algorithm>

namespace dcmcpp
{

FileMetaInfo::FileMetaInfo(std::vector<DataElement> dataElements)
    : FileMetaInfo("", std::move(dataElements))
{
}

FileMetaInfo::FileMetaInfo(std::string ppreamble, std::vector<DataElement> dataElements)
    : m_preamble(std::move(m_preamble)), m_dataElements(std::move(dataElements))
{
}

std::string FileMetaInfo::preamble() const
{
    return m_preamble;
}

UnsignedLong FileMetaInfo::groupLength() const
{
    const auto it = findDataElementByTag(m_dataElements, FileMetaInformationGroupLength);
    if (it != m_dataElements.cend())
        return std::get<UnsignedLong>(it->value);
    else
        throw std::runtime_error("No group length"); // TODO: proper exception
}

OtherByte FileMetaInfo::version() const
{
    const auto it = findDataElementByTag(m_dataElements, FileMetaInformationVersion);
    if (it != m_dataElements.cend())
        return std::get<OtherByte>(it->value);
    else
        throw std::runtime_error("No version"); // TODO: proper exception
}

UniqueIdentifier FileMetaInfo::mediaStorageSopClassUid() const
{
    const auto it = findDataElementByTag(m_dataElements, MediaStorageSopClassUid);
    if (it != m_dataElements.cend())
        return std::get<UniqueIdentifier>(it->value);
    else
        throw std::runtime_error("No media storage sop class uid"); // TODO: proper exception
}

UniqueIdentifier FileMetaInfo::mediaStorageSopInstanceUid() const
{
    const auto it = findDataElementByTag(m_dataElements, MediaStorageSopInstanceUid);
    if (it != m_dataElements.cend())
        return std::get<UniqueIdentifier>(it->value);
    else
        throw std::runtime_error("No media storage sop instance uid"); // TODO: proper exception
}

UniqueIdentifier FileMetaInfo::transferSyntaxUid() const
{
    const auto it = findDataElementByTag(m_dataElements, TransferSyntaxUid);
    if (it != m_dataElements.cend())
        return std::get<UniqueIdentifier>(it->value);
    else
        throw std::runtime_error("No transfer syntax uid"); // TODO: proper exception
}

UniqueIdentifier FileMetaInfo::implementationClassUid() const
{
    const auto it = findDataElementByTag(m_dataElements, ImplementationClassUid);
    if (it != m_dataElements.cend())
        return std::get<UniqueIdentifier>(it->value);
    else
        throw std::runtime_error("No implementation class uid"); // TODO: proper exception
}

std::string FileMetaInfo::implementationVersionName() const
{
    const auto it = findDataElementByTag(m_dataElements, ImplementationVersionName);
    if (it != m_dataElements.cend())
        return std::get<UniqueIdentifier>(it->value);
    else
        throw std::runtime_error("No implementation version name"); // TODO: proper exception
}

const DataElements FileMetaInfo::dataElements() const
{
    return m_dataElements;
}

}
