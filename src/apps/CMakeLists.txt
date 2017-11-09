project(Dump)

add_executable(${PROJECT_NAME} "")

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

include(Dump/CMakeLists.txt)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")