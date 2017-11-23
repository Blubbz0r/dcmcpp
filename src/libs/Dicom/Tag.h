#pragma once

#include <cstdint>
#include <string>

namespace dcmcpp
{

// TODO: doc
struct Tag
{
    uint16_t group;
    uint16_t element;
};

// TODO: doc
Tag tagFromString(std::string s);
// TODO: doc
std::string tagToString(const Tag& tag);

// TODO: provide other operators?
bool operator==(const Tag& lhs, const Tag& rhs);
bool operator!=(const Tag& lhs, const Tag& rhs);

// File Meta Elements (group 0002)
static constexpr Tag FileMetaInformationGroupLength{ 0x0002, 0x0000 };
static constexpr Tag FileMetaInformationVersion{ 0x0002, 0x0001 };
static constexpr Tag MediaStorageSopClassUid{ 0x0002, 0x0002 };
static constexpr Tag MediaStorageSopInstanceUid{ 0x0002, 0x0003 };
static constexpr Tag TransferSyntaxUid{ 0x0002, 0x0010 };
static constexpr Tag ImplementationClassUid{ 0x0002, 0x0012 };
static constexpr Tag ImplementationVersionName{ 0x0002, 0x0013 };
static constexpr Tag SourceApplicationEntityTitle{ 0x0002, 0x0016 };
static constexpr Tag SendingApplicationEntityTitle{ 0x0002, 0x0017 };
static constexpr Tag ReceivingApplicationEntityTitle{ 0x0002, 0x0018 };
static constexpr Tag PrivateInformationCreatorUid{ 0x0002, 0x0100 };
static constexpr Tag PrivateInformation{ 0x0002, 0x0102 };

// Data Elements (group 0008 and up)
static constexpr Tag SpecificCharacterSet{ 0x0008, 0x0005 };
static constexpr Tag ImageType{ 0x0008, 0x0008 };
static constexpr Tag InstanceCreationDate{ 0x0008, 0x0012 };
static constexpr Tag InstanceCreationTime{ 0x0008, 0x0013 };
static constexpr Tag InstanceCreatorUid{ 0x0008, 0x0014 };
static constexpr Tag SopClassUid{ 0x0008, 0x0016 };
static constexpr Tag SopInstanceUid{ 0x0008, 0x0018 };
static constexpr Tag StudyDate{ 0x0008, 0x0020 };
static constexpr Tag SeriesDate{ 0x0008, 0x0021 };
static constexpr Tag AcquisitionDate{ 0x0008, 0x0022 };
static constexpr Tag ContentDate{ 0x0008, 0x0023 };
static constexpr Tag StudyTime{ 0x0008, 0x0030 };
static constexpr Tag SeriesTime{ 0x0008, 0x0031 };
static constexpr Tag AcquisitionTime{ 0x0008, 0x0032 };
static constexpr Tag ContentTime{ 0x0008, 0x0033 };
static constexpr Tag AccessionNumber{ 0x0008, 0x0050 };
static constexpr Tag RetrieveAeTitle{ 0x0008, 0x0054 };
static constexpr Tag Modality{ 0x0008, 0x0060 };
static constexpr Tag Manufacturer{ 0x0008, 0x0070 };
static constexpr Tag InstitutionName{ 0x0008, 0x0080 };
static constexpr Tag ReferringPhysiciansName{ 0x0008, 0x0090 };
static constexpr Tag CodeValue{ 0x0008, 0x0100 };
static constexpr Tag CodingSchemeDesignator{ 0x0008, 0x0102 };
static constexpr Tag CodeMeaning{ 0x0008, 0x0104 };
static constexpr Tag CodingSchemeUid{ 0x0008, 0x010C };
static constexpr Tag CodingSchemeIdentificationSequence{ 0x0008, 0x0110 };
static constexpr Tag CodingSchemeName{ 0x0008, 0x0115 };
static constexpr Tag CodingSchemeResponsibleOrganization{ 0x0008, 0x0116 };
static constexpr Tag StationName{ 0x0008, 0x1010 };
static constexpr Tag StudyDescription{ 0x0008, 0x1030 };
static constexpr Tag PerformingPhysicianName{ 0x0008, 0x1050 };
static constexpr Tag NameOfPhysiciansReadingStudy{ 0x0008, 0x1060 };
static constexpr Tag OperatorsName{ 0x0008, 0x1070 };
static constexpr Tag ManufacturersModelName{ 0x0008, 0x1090 };
static constexpr Tag SeriesDescription{ 0x0008, 0x103E };
static constexpr Tag ReferencedPerformedProcedureStepSequence{ 0x0008, 0x1111 };
static constexpr Tag ReferencedSeriesSequence{ 0x0008, 0x1115 };
static constexpr Tag ReferencedSopClassUid{ 0x0008, 0x1150 };
static constexpr Tag ReferencedSopInstanceUid{ 0x0008, 0x1155 };
static constexpr Tag ReferencedSopSequence{ 0x0008, 0x1199 };

static constexpr Tag PatientsName{ 0x0010, 0x0010 };
static constexpr Tag PatientId{ 0x0010, 0x0020 };
static constexpr Tag PatientsBirthDate{ 0x0010, 0x0030 };
static constexpr Tag PatientsSex{ 0x0010, 0x0040 };

static constexpr Tag ContrastBolusAgent{ 0x0018, 0x0010 };
static constexpr Tag SliceThickness{ 0x0018, 0x0050 };
static constexpr Tag KVP{ 0x0018, 0x0060 };
static constexpr Tag DeviceSerialNumber{ 0x0018, 0x1000 };
static constexpr Tag ProtocolName{ 0x0018, 0x1030 };
static constexpr Tag GantryDetectorTilt{ 0x0018, 0x1120 };
static constexpr Tag XRayTubeCurrent{ 0x0018, 0x1151 };
static constexpr Tag Exposure{ 0x0018, 0x1152 };
static constexpr Tag FilterType{ 0x0018, 0x1160 };
static constexpr Tag PatientPosition{ 0x0018, 0x5100 };

static constexpr Tag StudyInstanceUid{ 0x0020, 0x000D };
static constexpr Tag SeriesInstanceUid{ 0x0020, 0x000E };
static constexpr Tag StudyId{ 0x0020, 0x0010 };
static constexpr Tag SeriesNumber{ 0x0020, 0x0011 };
static constexpr Tag AcquisitionNumber{ 0x0020, 0x0012 };
static constexpr Tag InstanceNumber{ 0x0020, 0x0013 };
static constexpr Tag PatientOrientation{ 0x0020, 0x0020 };
static constexpr Tag ImagePositionPatient{ 0x0020, 0x0032 };
static constexpr Tag ImageOrientationPatient{ 0x0020, 0x0037 };
static constexpr Tag FrameOfReferenceUID{ 0x0020, 0x0052 };
static constexpr Tag SliceLocation{ 0x0020, 0x1041 };

static constexpr Tag SamplesPerPixel{ 0x0028, 0x0002 };
static constexpr Tag PhotometricInterpretation{ 0x0028, 0x0004 };
static constexpr Tag Rows{ 0x0028, 0x0010 };
static constexpr Tag Columns{ 0x0028, 0x0011 };
static constexpr Tag PixelSpacing{ 0x0028, 0x0030 };
static constexpr Tag BitsAllocated{ 0x0028, 0x0100 };
static constexpr Tag BitsStored{ 0x0028, 0x0101 };
static constexpr Tag HighBit{ 0x0028, 0x0102 };
static constexpr Tag PixelRepresentation{ 0x0028, 0x0103 };
static constexpr Tag WindowCenter{ 0x0028, 0x0150 };
static constexpr Tag WindowWidth{ 0x0028, 0x0151 };
static constexpr Tag RescaleIntercept{ 0x0028, 0x0152 };
static constexpr Tag RescaleSlope{ 0x0028, 0x0153 };

static constexpr Tag RelationshipType{ 0x0040, 0xA010 };
static constexpr Tag ValueType{ 0x0040, 0xA040 };
static constexpr Tag ConceptNameCodeSequence{ 0x0040, 0xA043 };
static constexpr Tag ContinuityOfContent{ 0x0040, 0xA050 };
static constexpr Tag VerifyingOrganization{ 0x0040, 0xA027 };
static constexpr Tag VerificationDateTime{ 0x0040, 0xA030 };
static constexpr Tag VerifyingObserverSequence{ 0x0040, 0xA073 };
static constexpr Tag VerifyingObserverName{ 0x0040, 0xA075 };
static constexpr Tag VerifyingObserverIdentificationCodeSequence{ 0x0040, 0xA088 };
static constexpr Tag TextValue{ 0x0040, 0xA160 };
static constexpr Tag PerformedProcedureCodeSequence{ 0x0040, 0xA372 };
static constexpr Tag CurrentRequestedProcedureEvidenceSequence{ 0x0040, 0xA375 };
static constexpr Tag PertinentOtherEvidenceSequence{ 0x0040, 0xA385 };
static constexpr Tag CompletionFlag{ 0x0040, 0xA491 };
static constexpr Tag VerificationFlag{ 0x0040, 0xA493 };
static constexpr Tag ContentSequence{ 0x0040, 0xA730 };

static constexpr Tag ItemDelimitationItem{ 0xFFFE, 0xE00D };
static constexpr Tag SequenceDelimitationItem{ 0xFFFE, 0xE0DD };

}
