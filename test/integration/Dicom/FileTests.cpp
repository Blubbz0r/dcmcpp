#include "Dicom/File.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{

TEST(FileTests, isDicomFile_PathToValidDicomFile_ReturnsTrue)
{
    EXPECT_TRUE(isDicomFile(R"(C:\test\dcm)"));
}

TEST(FileTests, isDicomFile_InvalidPath_ReturnsFalse)
{
    EXPECT_FALSE(isDicomFile(R"(NOT_A_VALID_FILE_PATH@:;/\)"));
}

TEST(FileTests, isDicomFile_EmptyPath_ReturnsFalse)
{
    EXPECT_FALSE(isDicomFile(""));
}

TEST(FileTests, isDicomFile_PathToNoneDicomFile_ReturnsFalse)
{
    EXPECT_FALSE(isDicomFile(R"(C:\test\test.db)"));
}

// TODO: we need something more generic to allow easy test creation for different DICOM files
TEST(FileTests, readDicomFile_PathToValidDicomFile_ReturnsCorrectDicomFile)
{
    const auto dicomFile = readDicomFile(R"(C:\test\dcm)");

    const auto& metaInfo = dicomFile.metaInfo();
    EXPECT_THAT(metaInfo.preamble(), Eq(""));
    EXPECT_THAT(metaInfo.groupLength(), Eq(static_cast<UnsignedLong>(200)));
    EXPECT_THAT(metaInfo.version(), Eq(static_cast<OtherByte>(256)));
    EXPECT_THAT(metaInfo.mediaStorageSopClassUid(), Eq(UniqueIdentifier{ "1.2.840.10008.5.1.4.1.1.2" }));
    EXPECT_THAT(metaInfo.mediaStorageSopInstanceUid(), Eq(UniqueIdentifier{ "1.2.392.200036.9116.2.5.1.48.1215491070.1145609227.884753" }));
    EXPECT_THAT(metaInfo.transferSyntaxUid(), Eq(UniqueIdentifier{ "1.2.840.10008.1.2" }));
    EXPECT_THAT(metaInfo.implementationClassUid(), Eq(UniqueIdentifier{ "1.2.276.0.7230010.3.0.3.5.3" }));
    EXPECT_THAT(metaInfo.implementationVersionName(), Eq(UniqueIdentifier{ "OFFIS_DCMTK_353" }));
}

TEST(FileTests, readDicomFile_PathToValidDicomFile2_ReturnsCorrectDicomFile)
{
    const auto dicomFile = readDicomFile(R"(C:\test\dcm2)");

    const auto& metaInfo = dicomFile.metaInfo();
    EXPECT_THAT(metaInfo.preamble(), Eq(""));
    EXPECT_THAT(metaInfo.groupLength(), Eq(static_cast<UnsignedLong>(200)));
    EXPECT_THAT(metaInfo.version(), Eq(static_cast<OtherByte>(256)));
    EXPECT_THAT(metaInfo.mediaStorageSopClassUid(), Eq(UniqueIdentifier{ "1.2.840.10008.5.1.4.1.1.88.11" }));
    EXPECT_THAT(metaInfo.mediaStorageSopInstanceUid(), Eq(UniqueIdentifier{ "1.2.276.0.7230010.3.1.4.1787205428.166.1117461927.15" }));
    EXPECT_THAT(metaInfo.transferSyntaxUid(), Eq(UniqueIdentifier{ "1.2.840.10008.1.2.1" }));
    EXPECT_THAT(metaInfo.implementationClassUid(), Eq(UniqueIdentifier{ "1.2.276.0.7230010.3.0.3.5.3" }));
    EXPECT_THAT(metaInfo.implementationVersionName(), Eq(UniqueIdentifier{ "OFFIS_DCMTK_353" }));

    const auto& dataset = dicomFile.dataset();
    EXPECT_THAT(std::get<Date>(dataset.dataElement(InstanceCreationDate).value), Eq("20050530"));
    EXPECT_THAT(std::get<Time>(dataset.dataElement(InstanceCreationTime).value), Eq("160527"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataset.dataElement(InstanceCreatorUid).value), Eq("1.2.276.0.7230010.3.0.3.5.3"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataset.dataElement(SopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.88.11"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataset.dataElement(SopInstanceUid).value), Eq("1.2.276.0.7230010.3.1.4.1787205428.166.1117461927.15"));
    EXPECT_THAT(std::get<Date>(dataset.dataElement(StudyDate).value), Eq(""));
    EXPECT_THAT(std::get<Date>(dataset.dataElement(ContentDate).value), Eq("20050530"));
    EXPECT_THAT(std::get<Time>(dataset.dataElement(StudyTime).value), Eq(""));
    EXPECT_THAT(std::get<Time>(dataset.dataElement(ContentTime).value), Eq("160527"));
    EXPECT_THAT(std::get<ShortString>(dataset.dataElement(AccessionNumber).value), Eq(""));
    EXPECT_THAT(std::get<CodeString>(dataset.dataElement(Modality).value), Eq("SR"));
    EXPECT_THAT(std::get<LongString>(dataset.dataElement(Manufacturer).value), Eq(""));
    EXPECT_THAT(std::get<PersonName>(dataset.dataElement(ReferringPhysiciansName).value), Eq("Augustus Caesar^Gaius Iulius Octavianus"));
}

TEST(FileTests, readDicomFile_PathToValidDicomFile3_ReturnsCorrectDicomFile)
{
    const auto dicomFile = readDicomFile(R"(C:\test\dcm3)");

    const auto& metaInfo = dicomFile.metaInfo();
    EXPECT_THAT(metaInfo.preamble(), Eq(""));
    EXPECT_THAT(metaInfo.groupLength(), Eq(static_cast<UnsignedLong>(180)));
    EXPECT_THAT(metaInfo.version(), Eq(static_cast<OtherByte>(256)));
    EXPECT_THAT(metaInfo.mediaStorageSopClassUid(), Eq(UniqueIdentifier{ "1.2.840.10008.5.1.4.1.1.4" }));
    EXPECT_THAT(metaInfo.mediaStorageSopInstanceUid(), Eq(UniqueIdentifier{ "1.2.840.113619.2.1.2.139348932.2.9.602503126" }));
    EXPECT_THAT(metaInfo.transferSyntaxUid(), Eq(UniqueIdentifier{ "1.2.840.10008.1.2" }));
    EXPECT_THAT(metaInfo.implementationClassUid(), Eq(UniqueIdentifier{ "1.2.840.113619.6.5" }));
    EXPECT_THAT(metaInfo.implementationVersionName(), Eq(UniqueIdentifier{ "1_2_5" }));
    EXPECT_THAT(std::get<UniqueIdentifier>(metaInfo.dataElement(SourceApplicationEntityTitle).value), Eq(UniqueIdentifier{ "sdc21" }));
}

// TODO: add test with DICOM file containing all meta info

}
