target_sources(
    ${PROJECT_NAME}

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/LittleEndianTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/LittleEndianStreamReaderTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/StreamUtilsTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/StringUtilsTests.cpp"
)