project(UnitTests)

add_executable(
    ${PROJECT_NAME}

    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}

    dcmcpp

    CONAN_PKG::googletest
)

include(Dicom/CMakeLists.txt)
include(Utils/CMakeLists.txt)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")