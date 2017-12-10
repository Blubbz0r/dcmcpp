#pragma once

#include "DataElement.h"
#include "OtherByte.h"
#include "UniqueIdentifier.h"
#include "UnsignedLong.h"

namespace dcmcpp
{

/*!
    "DICOM header"
    present in every DICOM file
    see DICOM PS3.10
    Contains attributes of the tag group 0002
    Encoded using Explicit VR Little Endian
*/
class FileMetaInfo
{
public:
    explicit FileMetaInfo(DataElements dataElements);
    FileMetaInfo(std::string preamble, DataElements dataElements);

    // TODO: add notes from DICOM standard to documentation?

    // TODO: doc
    std::string preamble() const;

    // TODO: doc
    UnsignedLong groupLength() const;

    // TODO: doc
    OtherByte version() const;

    /*!
        Uniquely identifies the SOP Class associated with the Data Set. SOP Class UIDs allowed for
        media storage are specified in PS3.4 - Media Storage Application Profiles.

        * (0002,0002)
        * Type 1
    */
    UniqueIdentifier mediaStorageSopClassUid() const;

    /*!
        Uniquely identifies the SOP Instance associated with the Data Set placed in the file and
        following the File Meta Information.

        * (0002,0003)
        * Type 1
    */
    UniqueIdentifier mediaStorageSopInstanceUid() const;

    /*!
        Uniquely identifies the Transfer Syntax used to encode the following Data Set. This
        Transfer Syntax does not apply to the File Meta Information.

        * (0002.0010)
        * Type 1
    */ 
    UniqueIdentifier transferSyntaxUid() const;

    /*!
        Uniquely identifies the implementation that wrote this file and its content. It provides
        an unambiguous identification of the tpye of implementation that last wrote the file in
        the event of interchange problems. It follows the same policies as defined by PS3.7
        (association negotiation).

        * (0002, 0012)
        * Type 1
    */
    UniqueIdentifier implementationClassUid() const;

    /*!
        Identifies a version for an Implementation Class UID (0002,0012) using up to 16 characters
        of the reportoire identified in Section 8.5. It follows the same policies as defined by
        PS3.7 (association negotiation).

        * (0002,0013)
        * Type 3
    */
    std::string implementationVersionName() const; // TODO: use proper SH class / typedef once it exists?

    // TODO: doc
    const DataElements dataElements() const;

private:
    // TODO: we probably need to enforce that preamble is exactly 128 bytes
    std::string m_preamble;
    DataElements m_dataElements;
};

}
