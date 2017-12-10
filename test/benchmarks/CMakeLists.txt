project(Benchmarks)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(
    ${PROJECT_NAME}

    dcmcpp

    CONAN_PKG::google-benchmark
)

include(${CMAKE_CURRENT_LIST_DIR}/Dicom/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/Utils/CMakeLists.txt)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")