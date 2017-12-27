set(DICOM_INCLUDE_DIR ${INCLUDE_DIR}/dcmcpp/Dicom)

target_sources(
    ${PROJECT_NAME}

    PRIVATE
        "${DICOM_INCLUDE_DIR}/ApplicationEntity.h"
        "${DICOM_INCLUDE_DIR}/CodeString.h"
        "${DICOM_INCLUDE_DIR}/DataElement.h" "${CMAKE_CURRENT_LIST_DIR}/DataElement.cpp"
        "${DICOM_INCLUDE_DIR}/Dataset.h" "${CMAKE_CURRENT_LIST_DIR}/Dataset.cpp"
        "${DICOM_INCLUDE_DIR}/Date.h"
        "${DICOM_INCLUDE_DIR}/DateTime.h"
        "${DICOM_INCLUDE_DIR}/DecimalString.h"
        "${DICOM_INCLUDE_DIR}/Dictionary.h" "${CMAKE_CURRENT_LIST_DIR}/Dictionary.cpp"
        "${DICOM_INCLUDE_DIR}/File.h" "${CMAKE_CURRENT_LIST_DIR}/File.cpp"
        "${DICOM_INCLUDE_DIR}/FileMetaInfo.h" "${CMAKE_CURRENT_LIST_DIR}/FileMetaInfo.cpp"
        "${DICOM_INCLUDE_DIR}/IntegerString.h"
        "${DICOM_INCLUDE_DIR}/LongString.h"
        "${DICOM_INCLUDE_DIR}/OtherByte.h"
        "${DICOM_INCLUDE_DIR}/OtherWord.h"
        "${DICOM_INCLUDE_DIR}/PersonName.h"
        "${DICOM_INCLUDE_DIR}/Sequence.h"
        "${DICOM_INCLUDE_DIR}/ShortString.h"
        "${DICOM_INCLUDE_DIR}/ShortText.h"
        "${DICOM_INCLUDE_DIR}/Tag.h" "${CMAKE_CURRENT_LIST_DIR}/Tag.cpp"
        "${DICOM_INCLUDE_DIR}/Time.h"
        "${DICOM_INCLUDE_DIR}/TransferSyntax.h"
        "${DICOM_INCLUDE_DIR}/UniqueIdentifier.h" "${CMAKE_CURRENT_LIST_DIR}/UniqueIdentifier.cpp"
        "${DICOM_INCLUDE_DIR}/UnlimitedText.h"
        "${DICOM_INCLUDE_DIR}/UnsignedShort.h"
        "${DICOM_INCLUDE_DIR}/UnsignedLong.h"
        "${DICOM_INCLUDE_DIR}/ValueLength.h" "${CMAKE_CURRENT_LIST_DIR}/ValueLength.cpp"
        "${DICOM_INCLUDE_DIR}/ValueRepresentation.h" "${CMAKE_CURRENT_LIST_DIR}/ValueRepresentation.cpp"
)

include(CreateSubfolders)
CreateSubfolders("Dicom")