#include "dcmcpp/Utils/LittleEndianStreamReader.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp::LittleEndian
{

TEST(LittleEndianStreamReaderTests, readIntegral_StreamWithTwoBytes_ReturnsCorrectUint16)
{
    std::stringstream stream;
    stream << '\x4' << '\x6';
    EXPECT_THAT(readIntegral<uint16_t>(stream), Eq(1540));
}

TEST(LittleEndianStreamReaderTests, readIntegral_StreamWithFourBytes_ReturnsCorrectUint32)
{
    std::stringstream stream;
    stream << '\x3' << '\x8' << '\x0' << '\x5';
    EXPECT_THAT(readIntegral<uint32_t>(stream), Eq(83888131));
}

TEST(LittleEndianStreamReaderTests, readIntegral_StreamWithoutEnoughBytes_Throws)
{
    std::stringstream stream;
    stream << '\x4';
    EXPECT_THROW(readIntegral<uint16_t>(stream), std::runtime_error);
}

TEST(LittleEndianStreamReaderTests, readString_NoneEmptyStream_ReturnsCorrectString)
{
    std::stringstream stream;
    stream << "TEST";
    EXPECT_THAT(readString(stream, 4u), Eq("TEST"));
}

TEST(LittleEndianStreamReaderTests, readString_StreamWithoutEnoughBytes_Throws)
{
    std::stringstream stream;
    stream << 'T';
    EXPECT_THROW(readString(stream, 4u), std::runtime_error);
}

}
