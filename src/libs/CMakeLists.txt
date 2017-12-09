project(dcmcpp)

add_library(${PROJECT_NAME} "")

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}

    CONAN_PKG::GSL
)

include(Dicom/CMakeLists.txt)
include(Utils/CMakeLists.txt)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "libs")