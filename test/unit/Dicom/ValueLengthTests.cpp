#include "dcmcpp/Dicom/ValueLength.h"
#include "dcmcpp/Dicom/Dictionary.h"
#include "dcmcpp/Dicom/ValueRepresentation.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROBAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OB, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRODAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OD, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROFAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OF, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROLAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OL, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROWAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OW, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRSQAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::SQ, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRUCAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UC, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRUNAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UN, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRURAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UR, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRUTAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UT, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_OtherVRAndExplicitTransferSyntax_ReturnsFalse)
{
    EXPECT_FALSE(hasExtendedLengthEncoding(ValueRepresentation::AE, dict::ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_AnyVRAndImplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::AE, dict::ImplicitVRLittleEndian));
}

}