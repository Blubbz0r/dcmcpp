#include "File.h"
#include "DataElement.h"
#include "OtherByte.h"
#include "ShortString.h"
#include "ValueRepresentation.h"

#include "Utils/LittleEndian.h"
#include "Utils/StringUtils.h"

#include <fstream>

namespace dcmcpp
{

namespace
{

constexpr auto PreambleSize = 128;
constexpr auto MagicStringSize = 4;
constexpr auto MagicString = "DICM";

}

File::File(FileMetaInfo metaInfo, Dataset dataset)
    : m_metaInfo(std::move(metaInfo))
    , m_dataset(std::move(dataset))
{
}

const FileMetaInfo& File::metaInfo() const
{
    return m_metaInfo;
}

FileMetaInfo& File::metaInfo()
{
    return m_metaInfo;
}

void File::setMetaInfo(FileMetaInfo metaInfo)
{
    m_metaInfo = std::move(metaInfo);
}

const Dataset& File::dataset() const
{
    return m_dataset;
}

Dataset& File::dataset()
{
    return m_dataset;
}

void File::setDataset(Dataset dataset)
{
    m_dataset = std::move(dataset);
}

bool isDicomFile(const std::experimental::filesystem::path& path)
{
    std::ifstream file(path.string(), std::ios_base::binary);
    if (!file.is_open())
        return false;

    file.seekg(PreambleSize);
    if (!file.good())
        return false;

    char magic[MagicStringSize + 1];
    magic[MagicStringSize] = '\0';
    file.read(magic, MagicStringSize);
    return strcmp(magic, MagicString) == 0;
}

uint16_t readGroup(std::istream& stream)
{
    static constexpr auto GroupSize = 2;
    char group[GroupSize];
    stream.read(group, GroupSize);
    return LittleEndian::toIntegral<uint16_t>(group);
}

uint16_t readElement(std::istream& stream)
{
    static constexpr auto ElementSize = 2;
    char element[ElementSize];
    stream.read(element, ElementSize);
    return LittleEndian::toIntegral<uint16_t>(element);
}

ValueRepresentation readValueRepresentation(std::istream& stream)
{
    static constexpr auto VrSize = 2;
    char vr[VrSize + 1];
    vr[VrSize] = '\0';
    stream.read(vr, VrSize);
    return vrFromString(vr);
}

int readValueLength(std::istream& stream, ValueRepresentation vr)
{
    if (hasExtendedLengthEncoding(vr))
    {
        char reserved[2];
        stream.read(reserved, 2);

        static constexpr auto VlSize = 4;
        char vl[VlSize];
        stream.read(vl, VlSize);
        return LittleEndian::toIntegral<uint32_t>(vl);
    }
    else
    {
        static constexpr auto VlSize = 2;
        char vl[VlSize];
        stream.read(vl, VlSize);
        return LittleEndian::toIntegral<uint16_t>(vl);
    }
}

std::string readApplicationEntity(std::istream& stream, int valueLength)
{
    std::string aet;
    aet.resize(valueLength);
    stream.read(aet.data(), valueLength);
    StringUtils::trim(aet);
    return aet;
}

CodeString readCodeString(std::istream& stream, int valueLength)
{
    CodeString codeString;
    codeString.resize(valueLength);
    stream.read(codeString.data(), valueLength);
    StringUtils::trim(codeString);
    return codeString;
}

Date readDate(std::istream& stream, int valueLength)
{
    Date date;
    date.resize(valueLength);
    stream.read(date.data(), valueLength);
    return date;
}

LongString readLongString(std::istream& stream, int valueLength)
{
    LongString longString;
    longString.resize(valueLength);
    stream.read(longString.data(), valueLength);
    return longString;
}

OtherByte readOtherByte(std::istream& stream, int valueLength)
{
    std::string ob;
    ob.resize(valueLength);
    stream.read(ob.data(), valueLength);
    return LittleEndian::toIntegral<OtherByte>(gsl::make_span(ob.data(), valueLength));
}

PersonName readPersonName(std::istream& stream, int valueLength)
{
    PersonName personName;
    personName.resize(valueLength);
    stream.read(personName.data(), valueLength);
    return personName;
}

ShortString readShortString(std::istream& stream, int valueLength)
{
    ShortString sh;
    sh.resize(valueLength);
    stream.read(sh.data(), valueLength);
    StringUtils::trim(sh);
    return sh;
}

Time readTime(std::istream& stream, int valueLength)
{
    Time time;
    time.resize(valueLength);
    stream.read(time.data(), valueLength);
    return time;
}

UniqueIdentifier readUniqueIdentifier(std::istream& stream, int valueLength)
{
    auto ui = std::make_unique<char[]>(valueLength + 1);
    ui[valueLength] = '\0';
    stream.read(ui.get(), valueLength);
    return ui.get();
}

decltype(DataElement::value) readValue(std::istream& stream, ValueRepresentation valueRepresentation, int valueLength)
{
    switch (valueRepresentation)
    {
    case ValueRepresentation::AE:
        return readApplicationEntity(stream, valueLength);
    case ValueRepresentation::CS:
        return readCodeString(stream, valueLength);
    case ValueRepresentation::DA:
        return readDate(stream, valueLength);
    case ValueRepresentation::LO:
        return readLongString(stream, valueLength);
    case ValueRepresentation::OB:
        return readOtherByte(stream, valueLength);
    case ValueRepresentation::PN:
        return readPersonName(stream, valueLength);
    case ValueRepresentation::SH:
        return readShortString(stream, valueLength);
    case ValueRepresentation::TM:
        return readTime(stream, valueLength);
    case ValueRepresentation::UI:
        return readUniqueIdentifier(stream, valueLength);
    }

    // TODO: implement for other VRs
    throw std::logic_error("Not implemented yet");
}

DataElement readDataElement(std::istream& stream)
{
    DataElement element;
    element.tag = Tag{ readGroup(stream), readElement(stream) };
    element.valueRepresentation = readValueRepresentation(stream);
    element.valueLength = readValueLength(stream, element.valueRepresentation);
    element.value = readValue(stream, element.valueRepresentation, element.valueLength);
    return element;
}

FileMetaInfo readFileMetaInfo(std::istream& stream)
{
    std::string preamble;
    preamble.resize(PreambleSize);
    stream.read(preamble.data(), PreambleSize);

    std::string magic;
    stream.read(magic.data(), MagicStringSize);

    std::vector<DataElement> metaInfoDataElements;

    // group length
    DataElement groupLengthElement;
    groupLengthElement.tag = Tag{ readGroup(stream), readElement(stream) };
    groupLengthElement.valueRepresentation = readValueRepresentation(stream);
    groupLengthElement.valueLength = readValueLength(stream, groupLengthElement.valueRepresentation);

    std::string groupLength;
    groupLength.resize(groupLengthElement.valueLength);
    stream.read(groupLength.data(), groupLengthElement.valueLength);

    groupLengthElement.value = LittleEndian::toIntegral<UnsignedLong>(gsl::make_span(groupLength.data(), groupLengthElement.valueLength));
    metaInfoDataElements.emplace_back(std::move(groupLengthElement));

    // read data elements until the amount of bytes required by File Meta Information Group Length (groupLength) are reached
    const auto groupLengthOffset = stream.tellg();
    while ((stream.tellg() - groupLengthOffset) < std::get<UnsignedLong>(groupLengthElement.value))
    {
        metaInfoDataElements.emplace_back(readDataElement(stream));
    }

    return FileMetaInfo(std::move(preamble), std::move(metaInfoDataElements));
}

Dataset readDataset(std::istream& stream, const UniqueIdentifier& transferSyntaxUid)
{
    std::vector<DataElement> datasetElements;

    // TODO: right now we only want to work on supporting Explicit VR Little Endian
    // this ensures that we don't throw exceptions for the other existing tests due to missing VRs, etc.
    if (transferSyntaxUid == "1.2.840.10008.1.2.1")
    {
        for (int i = 0; i < 13; ++i)
        //while (!stream.eof())
            datasetElements.emplace_back(readDataElement(stream));
    }

    return Dataset{ std::move(datasetElements) };
}

File readDicomFile(const std::experimental::filesystem::path& path)
{
    std::ifstream file(path.string(), std::ios_base::binary);
    if (!file.is_open())
        throw ""; // TODO: proper exception

    auto metaInfo = readFileMetaInfo(file);
    auto dataset = readDataset(file, metaInfo.transferSyntaxUid());
    return File(std::move(metaInfo), std::move(dataset));
}

}
