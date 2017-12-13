target_sources(
    ${PROJECT_NAME}

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/FileTests.cpp"
)

include(CreateSubfolders)
CreateSubfolders("Dicom")