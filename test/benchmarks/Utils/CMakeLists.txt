target_sources(
    ${PROJECT_NAME}

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/StringUtils_BM.cpp"
)

include(CreateSubfolders)
CreateSubfolders("Utils")