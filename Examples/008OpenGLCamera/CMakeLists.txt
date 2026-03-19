cmake_minimum_required(VERSION 3.25)
project(008OpenGLCamera)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_executable(008OpenGLCamera "008 OpenGL Camera.cpp" "Shader.cpp" "Shader.h" "../../3rdparty/glad/src/gl.c"
        "InputHandler.cpp"
        "InputHandler.h"
        "Camera.cpp"
        "Camera.h"
        "Scancodes.h"
)

target_include_directories(008OpenGLCamera PRIVATE "../../3rdparty/sdl/include" "../../3rdparty/glad/include" "../../3rdparty/glm" "../../3rdparty/stb")

target_link_libraries(008OpenGLCamera PRIVATE SDL3::SDL3)

add_custom_command(TARGET 008OpenGLCamera PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:008OpenGLCamera>/assets/)

if(WIN32)
    add_custom_command(TARGET 008OpenGLCamera PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:008OpenGLCamera>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:008OpenGLCamera>/)
endif()