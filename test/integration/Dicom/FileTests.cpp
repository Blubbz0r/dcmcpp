#include "dcmcpp/Dicom/File.h"
#include "dcmcpp/Dicom/Dictionary.h"

#include "dcmcpp/Utils/StringUtils.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{

// TODO: replace hard-coded file paths
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
// random thoughts: how about CMake is deploying all DICOM files that should be tested to binary
// (sub) dir; each DICOM file comes together with a "dump-like" file settings the expected
// contents of the file (basically in an easier to digest DSL) and we have a generic test case
// that checks all those files?
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

    const auto& dataset = dicomFile.dataset();
    const auto& datasetElements = dataset.dataElements();
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, SpecificCharacterSet).value), Eq("ISO_IR 100"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, ImageType).value), Eq(R"(ORIGINAL\PRIMARY\AXIAL)"));
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
    const auto& datasetElements = dataset.dataElements();
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, InstanceCreationDate).value), Eq("20050530"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, InstanceCreationTime).value), Eq("160527"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, InstanceCreatorUid).value), Eq("1.2.276.0.7230010.3.0.3.5.3"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, SopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.88.11"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, SopInstanceUid).value), Eq("1.2.276.0.7230010.3.1.4.1787205428.166.1117461927.15"));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, StudyDate).value), Eq(""));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, ContentDate).value), Eq("20050530"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, StudyTime).value), Eq(""));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, ContentTime).value), Eq("160527"));
    EXPECT_THAT(std::get<ShortString>(dataElement(datasetElements, AccessionNumber).value), Eq(""));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, Modality).value), Eq("SR"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, Manufacturer).value), Eq(""));
    EXPECT_THAT(std::get<PersonName>(dataElement(datasetElements, ReferringPhysiciansName).value), Eq("Augustus Caesar^Gaius Iulius Octavianus"));

    {
        // Coding Scheme Identifier Sequence
        const auto& sequence = std::get<Sequence>(
            dataElement(datasetElements, CodingSchemeIdentificationSequence).value);

        ASSERT_THAT(sequence.items.size(), Eq(1u));

        const auto& firstSequenceItem = sequence.items[0];
        EXPECT_THAT(firstSequenceItem.dataElements.size(), Eq(5u));
        EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
        EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, CodingSchemeUid).value), Eq("1.2.276.0.7230010.3.0.0.1"));
        EXPECT_THAT(std::get<ShortText>(dataElement(firstSequenceItem.dataElements, CodingSchemeName).value), Eq("OFFIS DCMTK Coding Scheme"));
        EXPECT_THAT(std::get<ShortText>(dataElement(firstSequenceItem.dataElements, CodingSchemeResponsibleOrganization).value), Eq("Kuratorium OFFIS e.V., Escherweg 2, 26121 Oldenburg, Germany"));
        EXPECT_THAT(firstSequenceItem.dataElements.back().tag, Eq(ItemDelimitationItem));
    }

    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, StudyDescription).value), Eq("OFFIS Structured Reporting Templates"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, SeriesDescription).value), Eq("Fake Report, C. Iulius Caesar: De bello Gallico"));

    {
        // Referenced Performed Procedure Step Sequence
        const auto& sequence = std::get<Sequence>(
            dataElement(datasetElements, ReferencedPerformedProcedureStepSequence).value);

        EXPECT_THAT(sequence.items.size(), Eq(0u));
    }

    EXPECT_THAT(std::get<PersonName>(dataElement(datasetElements, PatientsName).value), Eq("Caesar^Gaius Iulius"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, PatientId).value), Eq(""));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, PatientsBirthDate).value), Eq(""));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, PatientsSex).value), Eq("M"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, StudyInstanceUid).value), Eq("1.2.276.0.7230010.3.4.1915765545.18030.917282194.11"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, SeriesInstanceUid).value), Eq("1.2.276.0.7230010.3.1.3.1787205428.166.1117461927.16"));
    EXPECT_THAT(std::get<ShortString>(dataElement(datasetElements, StudyId).value), Eq(""));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, SeriesNumber).value), Eq("1"));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, InstanceNumber).value), Eq("1"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, ValueType).value), Eq("CONTAINER"));

    {
        // Concept Name Code Sequence
        const auto& sequence =
            std::get<Sequence>(dataElement(datasetElements, ConceptNameCodeSequence).value);

        ASSERT_THAT(sequence.items.size(), Eq(1u));

        const auto& firstSequenceItem = sequence.items[0];
        EXPECT_THAT(firstSequenceItem.dataElements.size(), Eq(4u));
        EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_0.1"));
        EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
        EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("De bello Gallico"));
        EXPECT_THAT(firstSequenceItem.dataElements.back().tag, Eq(ItemDelimitationItem));
    }

    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, ContinuityOfContent).value), Eq("SEPARATE"));

    {
        // Verifying Observer Sequence
        const auto& sequence =
            std::get<Sequence>(dataElement(datasetElements, VerifyingObserverSequence).value);

        ASSERT_THAT(sequence.items.size(), Eq(1u));

        const auto& firstSequenceItem = sequence.items[0];
        EXPECT_THAT(firstSequenceItem.dataElements.size(), Eq(5u));
        EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, VerifyingOrganization).value), Eq("SPQR"));
        EXPECT_THAT(std::get<DateTime>(dataElement(firstSequenceItem.dataElements, VerificationDateTime).value), Eq("20050530160527"));
        EXPECT_THAT(std::get<PersonName>(dataElement(firstSequenceItem.dataElements, VerifyingObserverName).value), Eq("Augustus Caesar^Gaius Iulius Octavianus"));

        {
            // Verifying Observer Identification Code Sequence
            const auto& sequence =
                std::get<Sequence>(dataElement(firstSequenceItem.dataElements,
                                               VerifyingObserverIdentificationCodeSequence)
                                       .value);

            EXPECT_THAT(sequence.items.size(), Eq(0u));
        }

        EXPECT_THAT(firstSequenceItem.dataElements.back().tag, Eq(ItemDelimitationItem));
    }

    {
        // Performed Procedure Code Sequence
        const auto& sequence = std::get<Sequence>(
            dataElement(datasetElements, PerformedProcedureCodeSequence).value);

        EXPECT_THAT(sequence.items.size(), Eq(0u));
    }

    {
        // Current Requested Procedure Evidence Sequence
        const auto& sequence = std::get<Sequence>(
            dataElement(datasetElements, CurrentRequestedProcedureEvidenceSequence).value);

        ASSERT_THAT(sequence.items.size(), Eq(1u));

        const auto& firstSequenceItem = sequence.items[0];

        {
            // Referenced Series Sequence
            const auto& sequence = std::get<Sequence>(
                dataElement(firstSequenceItem.dataElements, ReferencedSeriesSequence).value);

            ASSERT_THAT(sequence.items.size(), Eq(1u));

            const auto& firstSequenceItem = sequence.items[0];
            EXPECT_THAT(std::get<ApplicationEntity>(dataElement(firstSequenceItem.dataElements, RetrieveAeTitle).value), Eq("DCMPSTATE"));

            {
                // Referenced SOP Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(firstSequenceItem.dataElements, ReferencedSopSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(1u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.7"));
                EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopInstanceUid).value), Eq("1.2.276.0.7230010.3.4.1915765545.18030.917282194.11.1.1"));
            }

            EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, SeriesInstanceUid).value), Eq("1.2.276.0.7230010.3.4.1915765545.18030.917282194.11.1"));
        }

        EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, StudyInstanceUid).value), Eq("1.2.276.0.7230010.3.4.1915765545.18030.917282194.11"));
    }

    {
        // Pertinent Other Evidence Sequence
        const auto& sequence = std::get<Sequence>(
            dataElement(datasetElements, PertinentOtherEvidenceSequence).value);

        ASSERT_THAT(sequence.items.size(), Eq(1u));

        const auto& firstSequenceItem = sequence.items[0];

        {
            // Referenced Series Sequence
            const auto& sequence = std::get<Sequence>(
                dataElement(firstSequenceItem.dataElements, ReferencedSeriesSequence).value);

            ASSERT_THAT(sequence.items.size(), Eq(1u));

            const auto& firstSequenceItem = sequence.items[0];
            EXPECT_THAT(std::get<ApplicationEntity>(dataElement(firstSequenceItem.dataElements, RetrieveAeTitle).value), Eq("DCMPSTATE"));

            {
                // Referenced SOP Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(firstSequenceItem.dataElements, ReferencedSopSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(1u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.11.1"));
                EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopInstanceUid).value), Eq("1.2.276.0.7230010.3.1.4.1707840890.221.974385531.18"));
            }

            EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, SeriesInstanceUid).value), Eq("1.2.276.0.7230010.3.1.4.1707840890.221.974385403.16"));
        }

        EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, StudyInstanceUid).value), Eq("1.2.276.0.7230010.3.4.1915765545.18030.917282194.11"));
    }

    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, CompletionFlag).value), Eq("COMPLETE"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, VerificationFlag).value), Eq("VERIFIED"));

    {
        // Content Sequence
        const auto& sequence =
            std::get<Sequence>(dataElement(datasetElements, ContentSequence).value);

        ASSERT_THAT(sequence.items.size(), Eq(2u));

        const auto& firstSequenceItem = sequence.items[0];

        {
            // Reference SOP Sequence
            const auto& sequence = std::get<Sequence>(
                dataElement(firstSequenceItem.dataElements, ReferencedSopSequence).value);

            ASSERT_THAT(sequence.items.size(), Eq(1u));

            const auto& firstSequenceItem = sequence.items[0];
            EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.7"));
            EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopInstanceUid).value), Eq("1.2.276.0.7230010.3.4.1915765545.18030.917282194.11.1.1"));

            {
                // Referenced SOP Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(firstSequenceItem.dataElements, ReferencedSopSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(1u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.11.1"));
                EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(firstSequenceItem.dataElements, ReferencedSopInstanceUid).value), Eq("1.2.276.0.7230010.3.1.4.1707840890.221.974385531.18"));
            }
        }

        EXPECT_THAT(std::get<CodeString>(dataElement(firstSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
        EXPECT_THAT(std::get<CodeString>(dataElement(firstSequenceItem.dataElements, ValueType).value), Eq("IMAGE"));

        {
            // Concept Name Code Sequence
            const auto& sequence = std::get<Sequence>(
                dataElement(firstSequenceItem.dataElements, ConceptNameCodeSequence).value);

            ASSERT_THAT(sequence.items.size(), Eq(1u));

            const auto& firstSequenceItem = sequence.items[0];
            EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_1.1"));
            EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
            EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("Image of the Author"));
        }

        const auto& secondSequenceItem = sequence.items[1];
        EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
        EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, ValueType).value), Eq("CONTAINER"));

        {
            // Concept Name Code Sequence
            const auto& sequence = std::get<Sequence>(
                dataElement(secondSequenceItem.dataElements, ConceptNameCodeSequence).value);

            ASSERT_THAT(sequence.items.size(), Eq(1u));

            const auto& firstSequenceItem = sequence.items[0];
            EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_2.1"));
            EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
            EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("Liber primus"));
        }

        EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, ContinuityOfContent).value), Eq("SEPARATE"));

        {
            // Content Sequence
            const auto& sequence = std::get<Sequence>(
                dataElement(secondSequenceItem.dataElements, ContentSequence).value);

            ASSERT_THAT(sequence.items.size(), Eq(2u));

            const auto& firstSequenceItem = sequence.items[0];
            EXPECT_THAT(std::get<CodeString>(dataElement(firstSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
            EXPECT_THAT(std::get<CodeString>(dataElement(firstSequenceItem.dataElements, ValueType).value), Eq("CONTAINER"));

            {
                // Concept Name Code Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(firstSequenceItem.dataElements, ConceptNameCodeSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(1u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_3.1"));
                EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("I"));
            }

            EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, ContinuityOfContent).value), Eq("SEPARATE"));

            {
                // Content Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(firstSequenceItem.dataElements, ContentSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(7u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(firstSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.1"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("1"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(firstSequenceItem.dataElements, TextValue).value), Eq("Gallia est omnis divisa in partes tres, quarum unam incolunt Belgae, aliam Aquitani, tertiam, qui ipsorum lingua Celtae, nostra Galli appellantur."));

                const auto& secondSequenceItem = sequence.items[1];
                EXPECT_THAT(std::get<ShortString>(dataElement(secondSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(secondSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(secondSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.2"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("2"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit."));

                const auto& thirdSequenceItem = sequence.items[2];
                EXPECT_THAT(std::get<ShortString>(dataElement(thirdSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(thirdSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(thirdSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.3"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("3"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit."));

                const auto& fourthSequenceItem = sequence.items[3];
                EXPECT_THAT(std::get<ShortString>(dataElement(fourthSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(fourthSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(fourthSequenceItem.dataElements, ConceptNameCodeSequence)
                        .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.4"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("4"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit."));

                const auto& fifthSequenceItem = sequence.items[4];
                EXPECT_THAT(std::get<ShortString>(dataElement(fifthSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(fifthSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(fifthSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.5"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("5"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit."));

                const auto& sixthSequenceItem = sequence.items[5];
                EXPECT_THAT(std::get<ShortString>(dataElement(sixthSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(sixthSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(sixthSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.6"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("6"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit."));

                const auto& seventhSequenceItem = sequence.items[6];
                EXPECT_THAT(std::get<ShortString>(dataElement(seventhSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<ShortString>(dataElement(seventhSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(seventhSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.7"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("7"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit."));
            }

            const auto& secondSequenceItem = sequence.items[1];
            EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
            EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, ValueType).value), Eq("CONTAINER"));

            {
                // Concept Name Code Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(secondSequenceItem.dataElements, ConceptNameCodeSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(1u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_3.2"));
                EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("II"));
            }

            EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, ContinuityOfContent).value), Eq("SEPARATE"));

            {
                // Content Sequence
                const auto& sequence = std::get<Sequence>(
                    dataElement(secondSequenceItem.dataElements, ContentSequence).value);

                ASSERT_THAT(sequence.items.size(), Eq(5u));

                const auto& firstSequenceItem = sequence.items[0];
                EXPECT_THAT(std::get<CodeString>(dataElement(firstSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<CodeString>(dataElement(firstSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(firstSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.1"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("1"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(firstSequenceItem.dataElements, TextValue).value), Eq("Apud Helvetios longe nobilissimus fuit et ditissimus Orgetorix. Is M. Messalla M. Pisone consulibus regni cupiditate inductus coniurationem nobilitatis fecit et civitati persuasit, ut de finibus suis cum omnibus copiis exirent:"));

                const auto& secondSequenceItem = sequence.items[1];
                EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<CodeString>(dataElement(secondSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(secondSequenceItem.dataElements, ConceptNameCodeSequence)
                        .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.2"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("2"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(secondSequenceItem.dataElements, TextValue).value), Eq("Perfacile esse, cum virtute omnibus praestarent, totius Galliae imperio potiri."));

                const auto& thirdSequenceItem = sequence.items[2];
                EXPECT_THAT(std::get<CodeString>(dataElement(thirdSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<CodeString>(dataElement(thirdSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(thirdSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.3"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("3"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(thirdSequenceItem.dataElements, TextValue).value), Eq("Id hoc facilius iis persuasit, quod undique loci natura Helvetii continentur: una ex parte flumine Rheno latissimo atque altissimo, qui agrum Helvetium a Germanis dividit, altera ex parte monte Iura altissimo, qui est inter Sequanos et Helvetios, tertia lacu Lemanno et flumine Rhodano, qui provinciam nostram ab Helvetiis dividit."));

                const auto& fourthSequenceItem = sequence.items[3];
                EXPECT_THAT(std::get<CodeString>(dataElement(fourthSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<CodeString>(dataElement(fourthSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(fourthSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.4"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("4"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(fourthSequenceItem.dataElements, TextValue).value), Eq("His rebus fiebat, ut et minus late vagarentur et minus facile finitimis bellum inferre possent; qua ex parte homines bellandi cupidi magno dolore afficiebantur. 5 Pro multitudine autem hominum et pro gloria belli atque fortitudinis angustos se fines habere arbitrabantur, qui in longitudinem milia passuum ducenta quadraginta, in latitudinem centum octoginta patebant."));

                const auto& fifthSequenceItem = sequence.items[4];
                EXPECT_THAT(std::get<CodeString>(dataElement(fifthSequenceItem.dataElements, RelationshipType).value), Eq("CONTAINS"));
                EXPECT_THAT(std::get<CodeString>(dataElement(fifthSequenceItem.dataElements, ValueType).value), Eq("TEXT"));

                {
                    // Concept Name Code Sequence
                    const auto& sequence = std::get<Sequence>(
                        dataElement(fifthSequenceItem.dataElements, ConceptNameCodeSequence)
                            .value);

                    ASSERT_THAT(sequence.items.size(), Eq(1u));

                    const auto& firstSequenceItem = sequence.items[0];
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodeValue).value), Eq("CH_4.5"));
                    EXPECT_THAT(std::get<ShortString>(dataElement(firstSequenceItem.dataElements, CodingSchemeDesignator).value), Eq("99_OFFIS_DCMTK"));
                    EXPECT_THAT(std::get<LongString>(dataElement(firstSequenceItem.dataElements, CodeMeaning).value), Eq("5"));
                }

                EXPECT_THAT(std::get<UnlimitedText>(dataElement(fifthSequenceItem.dataElements, TextValue).value), Eq("Pro multitudine autem hominum et pro gloria belli atque fortitudinis angustos se fines habere arbitrabantur, qui in longitudinem milia passuum ducenta quadraginta, in latitudinem centum octoginta patebant."));
            }
        }
    }
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
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(metaInfo.dataElements(), SourceApplicationEntityTitle).value), Eq(UniqueIdentifier{ "sdc21" }));
}

TEST(FileTests, readDicomFile_PathToValidDicomFile4_ReturnsCorrectDicomFile)
{
    const auto dicomFile = readDicomFile(R"(C:\test\dcm4)");

    const auto& metaInfo = dicomFile.metaInfo();
    EXPECT_THAT(metaInfo.preamble(), Eq(""));
    EXPECT_THAT(metaInfo.groupLength(), Eq(static_cast<UnsignedLong>(194)));
    EXPECT_THAT(metaInfo.version(), Eq(static_cast<OtherByte>(256)));
    EXPECT_THAT(metaInfo.mediaStorageSopClassUid(), Eq(UniqueIdentifier{ "1.2.840.10008.5.1.4.1.1.2" }));
    EXPECT_THAT(metaInfo.mediaStorageSopInstanceUid(), Eq(UniqueIdentifier{ "2.16.840.1.113662.2.1.4519.41582.4105152.419990505.410523251" }));
    EXPECT_THAT(metaInfo.transferSyntaxUid(), Eq(UniqueIdentifier{ "1.2.840.10008.1.2.1" }));
    EXPECT_THAT(metaInfo.implementationClassUid(), Eq(UniqueIdentifier{ "2.16.840.1.113662.2.1.1" }));
    EXPECT_THROW(metaInfo.implementationVersionName(), std::runtime_error);
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(metaInfo.dataElements(), SourceApplicationEntityTitle).value), Eq(UniqueIdentifier{ "PHOENIXSCP" }));

    const auto& dataset = dicomFile.dataset();
    const auto& datasetElements = dataset.dataElements();
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, SpecificCharacterSet).value), Eq("ISO_IR 100"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, ImageType).value), Eq(R"(ORIGINAL\PRIMARY\AXIAL)"));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, InstanceCreationDate).value), Eq("1999.05.05"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, InstanceCreationTime).value), Eq("10:52:34.530000"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, SopClassUid).value), Eq("1.2.840.10008.5.1.4.1.1.2"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, SopInstanceUid).value), Eq("2.16.840.1.113662.2.1.4519.41582.4105152.419990505.410523251"));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, StudyDate).value), Eq("1999.05.05"));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, SeriesDate).value), Eq("1999.05.05"));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, AcquisitionDate).value), Eq("1999.05.05"));
    EXPECT_THAT(std::get<Date>(dataElement(datasetElements, ContentDate).value), Eq("1999.05.05"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, StudyTime).value), Eq("10:52:34.530000"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, SeriesTime).value), Eq("10:52:34.530000"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, AcquisitionTime).value), Eq("10:52:34.530000"));
    EXPECT_THAT(std::get<Time>(dataElement(datasetElements, ContentTime).value), Eq("10:52:32.510000"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, Modality).value), Eq("CT"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, Manufacturer).value), Eq("Picker International, Inc."));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, InstitutionName).value), Eq("105 HOSPITAL"));
    EXPECT_THAT(std::get<PersonName>(dataElement(datasetElements, ReferringPhysiciansName).value), Eq("Anonymized"));
    EXPECT_THAT(std::get<ShortString>(dataElement(datasetElements, StationName).value), Eq("Picker CT"));
    EXPECT_THAT(std::get<ShortString>(dataElement(datasetElements, StudyDescription).value), Eq(""));
    EXPECT_THAT(std::get<PersonName>(dataElement(datasetElements, PerformingPhysicianName).value), Eq("Anonymized"));
    EXPECT_THAT(std::get<PersonName>(dataElement(datasetElements, NameOfPhysiciansReadingStudy).value), Eq("Anonymized"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, ManufacturersModelName).value), Eq("PQ5000"));
    EXPECT_THAT(std::get<PersonName>(dataElement(datasetElements, PatientsName).value), Eq("Anonymized"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, ContrastBolusAgent).value), Eq("C-"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, SliceThickness).value), Eq("10.0"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, KVP).value), Eq("120"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, DeviceSerialNumber).value), Eq("519"));
    EXPECT_THAT(std::get<LongString>(dataElement(datasetElements, ProtocolName).value), Eq("ADULT BRAIN/U"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, GantryDetectorTilt).value), Eq(".0"));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, XRayTubeCurrent).value), Eq("250"));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, Exposure).value), Eq("526"));
    EXPECT_THAT(std::get<ShortString>(dataElement(datasetElements, FilterType).value), Eq("0"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, PatientPosition).value), Eq("HFS"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, StudyInstanceUid).value), Eq("2.16.840.1.113662.2.1.1519.11582.1990505.1105152"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, SeriesInstanceUid).value), Eq("2.16.840.1.113662.2.1.2519.21582.2990505.2105152.2381633.20"));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, SeriesNumber).value), Eq("3513"));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, AcquisitionNumber).value), Eq("3513"));
    EXPECT_THAT(std::get<IntegerString>(dataElement(datasetElements, InstanceNumber).value), Eq("8"));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, PatientOrientation).value), Eq(R"(L\P)"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, ImagePositionPatient).value), Eq(R"(-1.197656e02\-3.997656e02\-2.800000e02)"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, ImageOrientationPatient).value), Eq(R"(1.000000e00\0.000000e00\0.000000e00\0.000000e00\1.000000e00\0.000000e00)"));
    EXPECT_THAT(std::get<UniqueIdentifier>(dataElement(datasetElements, FrameOfReferenceUID).value), Eq("2.16.840.1.113662.2.1.3519.31582.3990505.3105152"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, SliceLocation).value), Eq("280.00"));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, SamplesPerPixel).value), Eq(1));
    EXPECT_THAT(std::get<CodeString>(dataElement(datasetElements, PhotometricInterpretation).value), Eq("MONOCHROME2"));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, Rows).value), Eq(512));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, Columns).value), Eq(512));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, PixelSpacing).value), Eq(R"(0.46875\0.46875)"));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, BitsAllocated).value), Eq(16));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, BitsStored).value), Eq(16));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, HighBit).value), Eq(15));
    EXPECT_THAT(std::get<UnsignedShort>(dataElement(datasetElements, PixelRepresentation).value), Eq(1));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, WindowCenter).value), Eq("50"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, WindowWidth).value), Eq("75"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, RescaleIntercept).value), Eq("0"));
    EXPECT_THAT(std::get<DecimalString>(dataElement(datasetElements, RescaleSlope).value), Eq("1"));
}

// TODO: add test with DICOM file containing all meta info

}
