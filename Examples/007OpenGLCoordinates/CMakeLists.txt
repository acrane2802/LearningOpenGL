cmake_minimum_required(VERSION 3.25)
project(007OpenGLCoordinates)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(007OpenGLCoordinates "007 OpenGL Coordinates.cpp" "Shader.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(007OpenGLCoordinates PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm ../../3rdparty/stb)

target_link_libraries(007OpenGLCoordinates PRIVATE SDL2::SDL2main SDL2::SDL2)

add_custom_command(TARGET 007OpenGLCoordinates PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:007OpenGLCoordinates>/assets/)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(WIN32)
        add_custom_command(TARGET 007OpenGLCoordinates PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:007OpenGLCoordinates>/../../3rdparty/sdl2/SDL2d.dll $<TARGET_FILE_DIR:007OpenGLCoordinates>/)
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        add_custom_command(TARGET 007OpenGLCoordinates PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:007OpenGLCoordinates>/../../3rdparty/sdl2/SDL2.dll $<TARGET_FILE_DIR:007OpenGLCoordinates>/)
    endif()
endif()