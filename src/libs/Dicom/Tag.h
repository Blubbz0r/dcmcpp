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
static constexpr Tag InstanceCreationDate{ 0x0008, 0x0012 };
static constexpr Tag InstanceCreationTime{ 0x0008, 0x0013 };
static constexpr Tag InstanceCreatorUid{ 0x0008, 0x0014 };
static constexpr Tag SopClassUid{ 0x0008, 0x0016 };
static constexpr Tag SopInstanceUid{ 0x0008, 0x0018 };
static constexpr Tag StudyDate{ 0x0008, 0x0020 };
static constexpr Tag ContentDate{ 0x0008, 0x0023 };
static constexpr Tag StudyTime{ 0x0008, 0x0030 };
static constexpr Tag ContentTime{ 0x0008, 0x0033 };
static constexpr Tag AccessionNumber{ 0x0008, 0x0050 };
static constexpr Tag Modality{ 0x0008, 0x0060 };
static constexpr Tag Manufacturer{ 0x0008, 0x0070 };
static constexpr Tag ReferringPhysiciansName{ 0x0008, 0x0090 };

}
