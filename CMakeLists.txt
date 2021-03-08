cmake_minimum_required(VERSION 3.13)
project(buff C)

#LIBS
set(STB_INCLUDE ${CMAKE_SOURCE_DIR}/lib/stb)
set(FREETYPEGL_INCLUDE ${CMAKE_SOURCE_DIR}/lib/freetype-gl)

add_subdirectory(lib/glfw)

set(FREETYPE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/distance-field.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/edtaa3func.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/font-manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/text-buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/texture-atlas.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/texture-font.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/utf8-utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/vector.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/vertex-attribute.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype-gl/vertex-buffer.c
)

set(INCLUDE_DIRECTORIES "${STB_INCLUDE}" "${FREETYPEGL_INCLUDE}")
set(BUILD_FILES "${FREETYPE_SOURCES}")
#END LIBS

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h" "src/*.c")

message("------------------------------------------------------------------------------------------------------------------------------------------")
if (CMAKE_BUILD_TYPE MATCHES Debug)
endif()

if (CMAKE_BUILD_TYPE MATCHES Release)
endif()

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
message("CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message("------------------------------------------------------------------------------------------------------------------------------------------")

add_executable(${PROJECT_NAME} ${SOURCES} ${BUILD_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${FREETYPE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${INCLUDE_DIRECTORIES} "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} glfw ${FREETYPE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

#RES DIR
set(source "${CMAKE_SOURCE_DIR}/res")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/res")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)
#END RES DIR
