#include "dcmcpp/Dicom/ValueLength.h"
#include "dcmcpp/Dicom/Dictionary.h"
#include "dcmcpp/Dicom/ValueRepresentation.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROBAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OB, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRODAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OD, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROFAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OF, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROLAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OL, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VROWAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::OW, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRSQAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::SQ, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRUCAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UC, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRUNAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UN, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRURAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UR, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_VRUTAndExplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::UT, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_OtherVRAndExplicitTransferSyntax_ReturnsFalse)
{
    EXPECT_FALSE(hasExtendedLengthEncoding(ValueRepresentation::AE, ExplicitVRLittleEndian));
}

TEST(ValueLengthTests, hasExtendedLengthEncoding_AnyVRAndImplicitTransferSyntax_ReturnsTrue)
{
    EXPECT_TRUE(hasExtendedLengthEncoding(ValueRepresentation::AE, ImplicitVRLittleEndian));
}

}