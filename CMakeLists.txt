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

option(WITH_APPS "Whether or not to include the apps" ON)
option(WITH_TESTS "Whether or not to include the test and benchmark projects" ON)

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

# force highest warning level
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror")
endif()

add_subdirectory(src)

if (WITH_TESTS)
  add_subdirectory(test)
endif()