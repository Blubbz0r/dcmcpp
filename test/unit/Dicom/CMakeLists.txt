target_sources(
    ${PROJECT_NAME}

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/TagTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/UniqueIdentifierTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ValueLengthTests.cpp"
)

include(CreateSubfolders)
CreateSubfolders("Dicom")