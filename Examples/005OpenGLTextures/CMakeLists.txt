cmake_minimum_required(VERSION 3.25)
project(005OpenGLTextures)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(005OpenGLTextures "005 OpenGL Textures.cpp" "Shader.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(005OpenGLTextures PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm ../../3rdparty/stb)

target_link_libraries(005OpenGLTextures PRIVATE SDL3::SDL3)

add_custom_command(TARGET 005OpenGLTextures PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:005OpenGLTextures>/assets/)

if(WIN32)
    add_custom_command(TARGET 005OpenGLTextures PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:005OpenGLTextures>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:005OpenGLTextures>/)
endif()