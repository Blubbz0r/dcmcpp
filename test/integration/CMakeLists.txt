project(IntegrationTests)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(
    ${PROJECT_NAME}

    dcmcpp

    CONAN_PKG::googlemock
)

include(Dicom/CMakeLists.txt)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")