set(UTILS_INCLUDE_DIR ${INCLUDE_DIR}/dcmcpp/Utils)

target_sources(
    ${PROJECT_NAME}

    PRIVATE
        "${UTILS_INCLUDE_DIR}/Notification.h"
        "${UTILS_INCLUDE_DIR}/LittleEndian.h"
        "${UTILS_INCLUDE_DIR}/LittleEndianStreamReader.h" "${CMAKE_CURRENT_LIST_DIR}/LittleEndianStreamReader.cpp"
        "${UTILS_INCLUDE_DIR}/StreamUtils.h" "${CMAKE_CURRENT_LIST_DIR}/StreamUtils.cpp"
        "${UTILS_INCLUDE_DIR}/StringUtils.h" "${CMAKE_CURRENT_LIST_DIR}/StringUtils.cpp"
)

include(CreateSubfolders)
CreateSubfolders("Utils")
