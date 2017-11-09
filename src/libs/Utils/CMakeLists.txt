target_sources(
    ${PROJECT_NAME}

    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/Notification.h"
        "${CMAKE_CURRENT_LIST_DIR}/LittleEndian.h"
        "${CMAKE_CURRENT_LIST_DIR}/StringUtils.h"

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/StringUtils.cpp"
)