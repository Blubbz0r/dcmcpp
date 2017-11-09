#include "Utils/StringUtils.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{
namespace StringUtils
{

TEST(StringUtilsTests, split_StringContainingDelimeter_ReturnsCorrectResult)
{
    auto result = split("TEST,TEST2", ",");
    ASSERT_THAT(result.size(), Eq(2u));
    EXPECT_THAT(result[0], Eq("TEST"));
    EXPECT_THAT(result[1], Eq("TEST2"));
}

TEST(StringUtilsTests, split_StringContainingDelimeterWithEmptyToken_ResultContainsEmptyToken)
{
    auto result = split("TEST,TEST2,", ",");
    ASSERT_THAT(result.size(), Eq(3u));
    EXPECT_THAT(result[0], Eq("TEST"));
    EXPECT_THAT(result[1], Eq("TEST2"));
    EXPECT_THAT(result[2], Eq(""));
}

TEST(StringUtilsTests, split_EmptyString_ReturnsEmptyVector)
{
    auto result = split("", ",");
    EXPECT_TRUE(result.empty());
}

TEST(StringUtilsTests, split_StringNotContainingDelimeter_ReturnsVectorWithSingleElement)
{
    auto result = split("TEST,TEST2", "|");
    ASSERT_THAT(result.size(), Eq(1u));
    EXPECT_THAT(result[0], Eq("TEST,TEST2"));
}

TEST(StringUtilsTests, startsWith_CorrectStartingString_ReturnsTrue)
{
    EXPECT_TRUE(startsWith("TEST", "T"));
}

TEST(StringUtilsTests, startsWith_IncorrectStartingString_ReturnsFalse)
{
    EXPECT_FALSE(startsWith("TEST", "F"));
}

TEST(StringUtilsTests, startsWith_InputStringShorterThanStartString_ReturnsFalse)
{
    EXPECT_FALSE(startsWith("T", "TEST"));
}

TEST(StringUtilsTests, ltrim_StringStartingWithWhitespace_TrimsWhitespace)
{
    std::string startingWithWhitespace("  TEST  ");
    ltrim(startingWithWhitespace);
    EXPECT_THAT(startingWithWhitespace, Not(StartsWith(" ")));
}

TEST(StringUtilsTests, ltrim_EmptyString_DoesNotChangeString)
{
    std::string empty("");
    ltrim(empty);
    EXPECT_THAT(empty, Eq(""));
}

TEST(StringUtilsTests, ltrim_StringWithWhitespaceInTheMiddle_DoesNotChangeString)
{
    std::string whitespaceInMiddle("TEST TEST");
    ltrim(whitespaceInMiddle);
    EXPECT_THAT(whitespaceInMiddle, Eq("TEST TEST"));
}

TEST(StringUtilsTests, rtrim_StringEndingWithWhitespace_TrimsWhitespace)
{
    std::string endingWithWhitespace("  TEST  ");
    rtrim(endingWithWhitespace);
    EXPECT_THAT(endingWithWhitespace, Not(EndsWith(" ")));
}

TEST(StringUtilsTests, rtrim_EmptyString_DoesNotChangeString)
{
    std::string empty("");
    rtrim(empty);
    EXPECT_THAT(empty, Eq(""));
}

TEST(StringUtilsTests, rtrim_StringWithWhitespaceInTheMiddle_DoesNotChangeString)
{
    std::string whitespaceInMiddle("TEST TEST");
    rtrim(whitespaceInMiddle);
    EXPECT_THAT(whitespaceInMiddle, Eq("TEST TEST"));
}

TEST(StringUtilsTests, trim_StringStartingAndEndingWithWhitespace_TrimsWhitespace)
{
    std::string startingAndEndingWithWhitespace("  TEST  ");
    trim(startingAndEndingWithWhitespace);
    EXPECT_THAT(startingAndEndingWithWhitespace, Not(StartsWith(" ")));
    EXPECT_THAT(startingAndEndingWithWhitespace, Not(EndsWith(" ")));
}

TEST(StringUtilsTests, trim_EmptyString_DoesNotChangeString)
{
    std::string empty("");
    trim(empty);
    EXPECT_THAT(empty, Eq(""));
}

TEST(StringUtilsTests, trim_StringWithWhitespaceInTheMiddle_DoesNotChangeString)
{
    std::string whitespaceInMiddle("TEST TEST");
    trim(whitespaceInMiddle);
    EXPECT_THAT(whitespaceInMiddle, Eq("TEST TEST"));
}

TEST(StringUtilsTests, intToHex_WithInteger_ReturnsCorrectResult)
{
    EXPECT_THAT(intToHex(20), Eq("0014"));
}

}
}
