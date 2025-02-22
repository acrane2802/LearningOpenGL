cmake_minimum_required(VERSION 3.25)
project(002OpenGLRectangle)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(002OpenGLRectangle "002 OpenGL Rectangle.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(002OpenGLRectangle PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(002OpenGLRectangle PRIVATE SDL3::SDL3)

add_custom_command(TARGET 002OpenGLRectangle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:002OpenGLRectangle>/assets/)

if(WIN32)
    add_custom_command(TARGET 002OpenGLRectangle PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:002OpenGLRectangle>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:002OpenGLRectangle>/)
endif()