target_sources(
    ${PROJECT_NAME}

    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/Notification.h"
        "${CMAKE_CURRENT_LIST_DIR}/LittleEndian.h"
        "${CMAKE_CURRENT_LIST_DIR}/LittleEndianStreamReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/StreamUtils.h"
        "${CMAKE_CURRENT_LIST_DIR}/StringUtils.h"

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/LittleEndianStreamReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/StreamUtils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/StringUtils.cpp"
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h")