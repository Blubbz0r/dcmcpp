#include "Utils/LittleEndian.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp::LittleEndian
{

TEST(LittleEndianTests, toIntegral_ToUint16WithValidCharArray_ReturnsCorrectResult)
{
    constexpr char input[2] = { '\x4', '\x2' };
    EXPECT_THAT(toIntegral<uint16_t>(input), Eq(516));
}

TEST(LittleEndianTests, toIntegral_ToUint32WithValidCharArray_ReturnsCorrectResult)
{
    constexpr char input[4] = { '\x2', '\0', '\x4', '\0' };
    EXPECT_THAT(toIntegral<uint32_t>(input), Eq(262146));
}

// TODO: can we test the static_assert?

}
