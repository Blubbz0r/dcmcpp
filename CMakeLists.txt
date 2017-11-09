project(dcmcpp)

cmake_minimum_required(VERSION 3.1.3)

# setup conan dependencies
set(CONAN_BUILD_INFO_PATH ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
mark_as_advanced(${CONAN_BUILD_INFO_PATH})
if (EXISTS ${CONAN_BUILD_INFO_PATH})
    include(${CONAN_BUILD_INFO_PATH})
    conan_basic_setup(TARGETS)
else()
    message(FATAL_ERROR "Please run conan install first (conanbuildfileinfo.cmake needs to be found here '${CONAN_BUILD_INFO_PATH}')")
endif()

# Turn on using solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# TODO: VS only right now
# TODO: CMake will properly support this for VS from version 3.10 only
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

add_subdirectory(src)
add_subdirectory(test)