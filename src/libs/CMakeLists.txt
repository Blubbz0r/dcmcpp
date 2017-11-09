project(dcmcpp)

add_library(${PROJECT_NAME} "")

# TODO: provide via conan
target_sources(
    ${PROJECT_NAME}

    PUBLIC
        gsl/gsl
        gsl/gsl_algorithm
        gsl/gsl_assert
        gsl/gsl_byte
        gsl/gsl_util
        gsl/multi_span
        gsl/pointers
        gsl/span
        gsl/string_span
)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

include(Dicom/CMakeLists.txt)
include(Utils/CMakeLists.txt)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")