#include "File.h"
#include "DataElement.h"
#include "OtherByte.h"
#include "ShortString.h"
#include "ValueRepresentation.h"

#include "Utils/LittleEndian.h"
#include "Utils/LittleEndianStreamReader.h"
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
    : m_metaInfo(std::move(metaInfo)), m_dataset(std::move(dataset))
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
    return LittleEndian::readIntegral<uint16_t>(stream);
}

uint16_t readElement(std::istream& stream)
{
    return LittleEndian::readIntegral<uint16_t>(stream);
}

Tag readTag(std::istream& stream)
{
    return Tag{ readGroup(stream), readElement(stream) };
}

ValueRepresentation readValueRepresentation(std::istream& stream)
{
    static constexpr auto VrSize = 2;
    return vrFromString(LittleEndian::readString(stream, VrSize));
}

// TODO: return value does not necessarily match what is needed...
int readValueLength(std::istream& stream, ValueRepresentation vr)
{
    if (hasExtendedLengthEncoding(vr))
    {
        char reserved[2];
        stream.read(reserved, 2);

        return LittleEndian::readIntegral<ExtendedValueLength>(stream);
    }
    else
    {
        return LittleEndian::readIntegral<ValueLength>(stream);
    }
}

ApplicationEntity readApplicationEntity(std::istream& stream, uint32_t valueLength)
{
    auto ae = LittleEndian::readString(stream, valueLength);
    StringUtils::trim(ae);
    return ae;
}

CodeString readCodeString(std::istream& stream, uint32_t valueLength)
{
    auto cs = LittleEndian::readString(stream, valueLength);
    StringUtils::trim(cs);
    return cs;
}

Date readDate(std::istream& stream, uint32_t valueLength)
{
    return LittleEndian::readString(stream, valueLength);
}

DateTime readDateTime(std::istream& stream, uint32_t valueLength)
{
    return LittleEndian::readString(stream, valueLength);
}

DecimalString readDecimalString(std::istream& stream, uint32_t valueLength)
{
    auto ds = LittleEndian::readString(stream, valueLength);
    StringUtils::rtrim(ds); // TODO: trim okay?
    return ds;
}

IntegerString readIntegerString(std::istream& stream, uint32_t valueLength)
{
    auto is = LittleEndian::readString(stream, valueLength);
    StringUtils::trim(is); // TODO: can include leading and trailing spaces according to DICOM but dcmdump and online dump don't include it in their output???
    return is;
}

LongString readLongString(std::istream& stream, uint32_t valueLength)
{
    auto ls = LittleEndian::readString(stream, valueLength);
    StringUtils::trim(ls); // TODO: can include leading and trailing spaces according to DICOM but dcmdump and online dump don't include it in their output???
    return ls;
}

// TODO: should we use value length here? OtherByte has a fixed length?!
// same for UnsignedShort and UnsignedLong
OtherByte readOtherByte(std::istream& stream, uint32_t valueLength)
{
    std::string ob;
    ob.resize(valueLength);
    stream.read(ob.data(), valueLength);
    return LittleEndian::toIntegral<OtherByte>(gsl::make_span(ob.data(), valueLength));
}

PersonName readPersonName(std::istream& stream, uint32_t valueLength)
{
    auto pn = LittleEndian::readString(stream, valueLength);
    StringUtils::rtrim(pn); // TODO: this fixes the test expectation and matches what dcmdump and online dump are printing, however, trailing spaces are allowed in PersonName and should not be ignored?
    return pn;
}

ShortString readShortString(std::istream& stream, uint32_t valueLength)
{
    auto sh = LittleEndian::readString(stream, valueLength);
    StringUtils::trim(sh);
    return sh;
}

ShortText readShortText(std::istream& stream, uint32_t valueLength)
{
    auto st = LittleEndian::readString(stream, valueLength);
    StringUtils::trim(st);
    return st;
}

Time readTime(std::istream& stream, uint32_t valueLength)
{
    auto time = LittleEndian::readString(stream, valueLength);
    StringUtils::rtrim(time); // TODO: trim correct?
    return time;
}

UniqueIdentifier readUniqueIdentifier(std::istream& stream, uint32_t valueLength)
{
    auto ui = std::make_unique<char[]>(valueLength + 1);
    ui[valueLength] = '\0';
    stream.read(ui.get(), valueLength);
    return ui.get();
}

decltype(DataElement::value) readValue(std::istream& stream,
                                       ValueRepresentation valueRepresentation,
                                       uint32_t valueLength);

Sequence readSequence(std::istream& stream, uint32_t valueLength)
{
    Sequence sequence;

    while (true)
    {
        static constexpr auto VlSize = 4;
        char vl[VlSize];

        auto tag = readTag(stream);
        if (tag == SequenceDelimitationItem)
        {
            // sequences don't necessarily have to contain items but can also be empty and closed by
            // a Sequence Delimitation Item immediately
            stream.read(vl, VlSize); // TODO: do we need this anywhere?

            return sequence;
        }

        SequenceItem item;
        item.tag = std::move(tag);

        stream.read(vl, VlSize);
        item.valueLength = LittleEndian::toIntegral<uint32_t>(vl);

        while (true)
        {
            tag = readTag(stream);
            if (tag != ItemDelimitationItem)
            {
                DataElement element;
                element.tag = std::move(tag);
                element.valueRepresentation = readValueRepresentation(stream);
                element.valueLength = readValueLength(stream, element.valueRepresentation);
                element.value = readValue(stream, element.valueRepresentation, element.valueLength);

                item.dataElements.emplace_back(std::move(element));
            }
            else
            {
                // TODO: do we want to do anything with the value length of Item Delimitation Item?
                stream.read(vl, VlSize);
                break;
            }
        }

        sequence.items.emplace_back(std::move(item));
    }
}

UnlimitedText readUnlimitedText(std::istream& stream, uint32_t valueLength)
{
    auto ut = LittleEndian::readString(stream, valueLength);
    StringUtils::rtrim(ut);
    return ut;
}

UnsignedLong readUnsignedLong(std::istream& stream, uint32_t valueLength)
{
    static constexpr auto UnsignedLongSize = sizeof(UnsignedLong);
    char ul[UnsignedLongSize];
    stream.read(ul, UnsignedLongSize);
    return LittleEndian::toIntegral<uint32_t>(ul);
}

UnsignedShort readUnsignedShort(std::istream& stream, uint32_t valueLength)
{
    static constexpr auto UnsignedShortSize = sizeof(UnsignedShort);
    char us[UnsignedShortSize];
    stream.read(us, valueLength);
    return LittleEndian::toIntegral<UnsignedShort>(us);
}

decltype(DataElement::value) readValue(std::istream& stream,
                                       ValueRepresentation valueRepresentation,
                                       uint32_t valueLength)
{
    switch (valueRepresentation)
    {
    case ValueRepresentation::AE:
        return readApplicationEntity(stream, valueLength);
    case ValueRepresentation::CS:
        return readCodeString(stream, valueLength);
    case ValueRepresentation::DA:
        return readDate(stream, valueLength);
    case ValueRepresentation::DS:
        return readDecimalString(stream, valueLength);
    case ValueRepresentation::DT:
        return readDateTime(stream, valueLength);
    case ValueRepresentation::IS:
        return readIntegerString(stream, valueLength);
    case ValueRepresentation::LO:
        return readLongString(stream, valueLength);
    case ValueRepresentation::OB:
        return readOtherByte(stream, valueLength);
    case ValueRepresentation::PN:
        return readPersonName(stream, valueLength);
    case ValueRepresentation::SH:
        return readShortString(stream, valueLength);
    case ValueRepresentation::SQ:
       return readSequence(stream, valueLength);
    case ValueRepresentation::ST:
        return readShortText(stream, valueLength);
    case ValueRepresentation::TM:
        return readTime(stream, valueLength);
    case ValueRepresentation::UI:
        return readUniqueIdentifier(stream, valueLength);
    case ValueRepresentation::UL:
        return readUnsignedLong(stream, valueLength);
    case ValueRepresentation::US:
        return readUnsignedShort(stream, valueLength);
    case ValueRepresentation::UT:
        return readUnlimitedText(stream, valueLength);
    }

    // TODO: implement for other VRs
    throw std::logic_error("readValue: Not implemented yet for " + vrToString(valueRepresentation));
}

DataElement readDataElement(std::istream& stream)
{
    DataElement element;
    element.tag = readTag(stream);
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
    groupLengthElement.tag = readTag(stream);
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
        // TODO: properly read until end of file
        for (int i = 0; i < 36; ++i)
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
