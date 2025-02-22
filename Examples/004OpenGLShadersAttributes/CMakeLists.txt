cmake_minimum_required(VERSION 3.25)
project(004OpenGLShadersAttributes)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(004OpenGLShadersAttributes "004 OpenGL Shaders Attributes.cpp" "Shader.h" "Shader.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(004OpenGLShadersAttributes PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(004OpenGLShadersAttributes PRIVATE SDL3::SDL3)

add_custom_command(TARGET 004OpenGLShadersAttributes PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:004OpenGLShadersAttributes>/assets/)

if(WIN32)
    add_custom_command(TARGET 004OpenGLShadersAttributes PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:004OpenGLShadersAttributes>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:004OpenGLShadersAttributes>/)
endif()