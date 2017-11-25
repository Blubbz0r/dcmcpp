#include "Dicom/Tag.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{

TEST(TagTests, tagFromString_CommaSeparatedTag_ReturnsCorrectTag)
{
    auto tag = tagFromString("0008,0010");
    EXPECT_THAT(tag.group, Eq(8));
    EXPECT_THAT(tag.element, Eq(10));
}

TEST(TagTests, tagFromString_CommaSeparatedTagInBraces_ReturnsCorrectTag)
{
    auto tag = tagFromString("(0002,0001)");
    EXPECT_THAT(tag.group, Eq(2));
    EXPECT_THAT(tag.element, Eq(1));
}

TEST(TagTests, tagToString_ReturnsProperString)
{
    EXPECT_THAT(tagToString(Tag{ 0x002, 0x0016 }), Eq("(0002,0016)"));
}

TEST(TagTests, isMetaInfo_TagFromGroup0002_ReturnsTrue)
{
    EXPECT_TRUE(isMetaInfo(Tag{ 0x0002, 0x0000 }));
}

TEST(TagTests, isMetaInfo_TagFromGroupOtherThan0002_ReturnsFalse)
{
    EXPECT_FALSE(isMetaInfo(Tag{ 0x0008, 0x0000 }));
}

}
