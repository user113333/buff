cmake_minimum_required(VERSION 3.12)
project(buff)

set(CMAKE_SUPPRESS_REGENERATION ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR} "src/*.c")

set(SOURCES ${SOURCES} "lib/glad/src/glad.c")

include_directories(
    "src/"
    "lib/"
    "lib/gl3w/include"
    "lib/glfw/include"
    "lib/fontstash/src"
    "lib/glad/include"
)

add_subdirectory(lib/glfw)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glfw)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    # target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG=1)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RES_PATH="${CMAKE_SOURCE_DIR}/res/")
endif()
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE=1)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RES_PATH="res/")

    # Copy res dir
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "${CMAKE_CURRENT_BINARY_DIR}/res"
    )
endif()
