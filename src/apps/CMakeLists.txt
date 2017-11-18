project(Dump)

add_executable(${PROJECT_NAME} "")

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}

    dcmcpp
)

include(Dump/CMakeLists.txt)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")