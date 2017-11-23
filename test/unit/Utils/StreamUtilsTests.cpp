#include "Utils/StreamUtils.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp::StreamUtils
{

TEST(StringUtilsTests, availableBytes_EmptyStream_ReturnsZero)
{
    std::stringstream stream;
    EXPECT_THAT(availableBytes(stream), Eq(0u));
}

TEST(StringUtilsTests, availableBytes_NoneEmptyStream_ReturnsCorrectResult)
{
    std::stringstream stream;
    stream << '\x1' << '\x2';
    EXPECT_THAT(availableBytes(stream), Eq(2u));
}

TEST(StringUtilsTests, availableBytes_NoneEmptyStream_DoesNotAlterCurrentStreamPosition)
{
    std::stringstream stream;
    stream << '\x5' << '\x1' << 'a';
    stream.seekg(1);

    auto pos = stream.tellg();
    availableBytes(stream);
    EXPECT_THAT(stream.tellg(), Eq(pos));
}

}
