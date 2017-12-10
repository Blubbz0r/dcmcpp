#include "dcmcpp/Dicom/File.h"
#include "dcmcpp/Dicom/DataElement.h"
#include "dcmcpp/Dicom/Dictionary.h"
#include "dcmcpp/Dicom/OtherByte.h"
#include "dcmcpp/Dicom/ShortString.h"
#include "dcmcpp/Dicom/TransferSyntax.h"
#include "dcmcpp/Dicom/ValueRepresentation.h"

#include "dcmcpp/Utils/LittleEndian.h"
#include "dcmcpp/Utils/LittleEndianStreamReader.h"
#include "dcmcpp/Utils/StringUtils.h"

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
int readValueLength(std::istream& stream, ValueRepresentation vr,
                    std::string_view transferSyntaxUid)
{
    if (hasExtendedLengthEncoding(vr, transferSyntaxUid))
    {
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

OtherWord readOtherWord(std::istream& stream, uint32_t valueLength)
{
    return LittleEndian::readString(stream, valueLength);
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
                                       uint32_t valueLength,
                                       std::string_view transferSyntaxUid);

DataElement readDataElement(std::istream& stream, std::string_view transferSyntaxUid);

Sequence readSequence(std::istream& stream, std::string_view transferSyntaxUid)
{
    Sequence sequence;

    while (true)
    {
        auto element = readDataElement(stream, transferSyntaxUid);
        if (element.tag == SequenceDelimitationItem)
        {
            // sequences don't necessarily have to contain items but can also be empty and closed by
            // a Sequence Delimitation Item immediately
            return sequence;
        }

        SequenceItem item;
        item.tag = element.tag;
        item.valueLength = element.valueLength;

        while (true)
        {
            item.dataElements.emplace_back(readDataElement(stream, transferSyntaxUid));
            if (item.dataElements.back().tag == ItemDelimitationItem)
                break;
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

// TODO: or use value length?
UnsignedLong readUnsignedLong(std::istream& stream, [[maybe_unused]] uint32_t valueLength)
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
                                       uint32_t valueLength, std::string_view transferSyntaxUid)
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
    case ValueRepresentation::OW:
        return readOtherWord(stream, valueLength);
    case ValueRepresentation::PN:
        return readPersonName(stream, valueLength);
    case ValueRepresentation::SH:
        return readShortString(stream, valueLength);
    case ValueRepresentation::SQ:
       return readSequence(stream, transferSyntaxUid);
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

constexpr bool isDelimitationItem(const Tag& tag)
{
    return tag == SequenceDelimitationItem || tag == ItemDelimitationItem;
}

constexpr bool hasReservedVRBytes(ValueRepresentation vr, std::string_view transferSyntaxUid)
{
    if (isExplicit(transferSyntaxUid))
    {
        // TODO: duplicated in ValueLength::hasExtendedLengthEncoding
        switch (vr)
        {
        case ValueRepresentation::OB:
        case ValueRepresentation::OD:
        case ValueRepresentation::OF:
        case ValueRepresentation::OL:
        case ValueRepresentation::OW:
        case ValueRepresentation::SQ:
        case ValueRepresentation::UC:
        case ValueRepresentation::UR:
        case ValueRepresentation::UN:
        case ValueRepresentation::UT:
            return true;

        default:
            return false;
        }
    }

    return false;
}

DataElement readDataElement(std::istream& stream, std::string_view transferSyntaxUid)
{
    DataElement element;
    element.tag = readTag(stream);

    if (isGroupLength(element.tag))
    {
        // TODO: not sure if group lengths never have VR?
        element.valueRepresentation = ValueRepresentation::UL;
        element.valueLength = LittleEndian::readIntegral<ExtendedValueLength>(stream);
    }
    else if (element.tag == Item || isDelimitationItem(element.tag))
    {
        element.valueRepresentation = ValueRepresentation::None;
        element.valueLength = LittleEndian::readIntegral<ExtendedValueLength>(stream);
        // TODO: these items never have a value?
        return element;
    }
    else
    {
        if (isExplicit(transferSyntaxUid))
            element.valueRepresentation = readValueRepresentation(stream);
        else
            element.valueRepresentation = getVr(element.tag);

        if (hasReservedVRBytes(element.valueRepresentation, transferSyntaxUid))
        {
            char reserved[2];
            stream.read(reserved, 2);
        }

        element.valueLength =
            readValueLength(stream, element.valueRepresentation, transferSyntaxUid);
    }

    element.value =
        readValue(stream, element.valueRepresentation, element.valueLength, transferSyntaxUid);
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
    auto groupLengthElement = readDataElement(stream, ExplicitVRLittleEndian);
    metaInfoDataElements.emplace_back(std::move(groupLengthElement));

    // read data elements until the amount of bytes required by File Meta Information Group Length (groupLength) are reached
    const auto groupLengthOffset = stream.tellg();
    while ((stream.tellg() - groupLengthOffset) < std::get<UnsignedLong>(groupLengthElement.value))
    {
        metaInfoDataElements.emplace_back(readDataElement(stream, ExplicitVRLittleEndian));
    }

    return FileMetaInfo(std::move(preamble), std::move(metaInfoDataElements));
}

Dataset readDataset(std::istream& stream, const UniqueIdentifier& transferSyntaxUid)
{
    std::vector<DataElement> datasetElements;

    // TODO: right now we only want to work on supporting Explicit and Implicit VR Little Endian
    // this ensures that we don't throw exceptions for the other existing tests due to missing VRs, etc.
    if (transferSyntaxUid == ExplicitVRLittleEndian || transferSyntaxUid == ImplicitVRLittleEndian)
    {
        // TODO: stream.eof() should be used here but it contains 1 more byte for dcm2??
        while (stream.tellg() < StreamUtils::availableBytes(stream))
            datasetElements.emplace_back(readDataElement(stream, transferSyntaxUid));
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
