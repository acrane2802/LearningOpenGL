cmake_minimum_required(VERSION 3.25)
project(010OpenGLBasicLighting)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    add_definitions(-DPLATFORM_WINDOWS)
endif(WIN32)

if (UNIX AND NOT APPLE)
    add_definitions(-DPLATFORM_LINUX)
endif(UNIX AND NOT APPLE)

if (UNIX AND APPLE)
    add_definitions(-DPLATFORM_MACOS)
endif(UNIX AND APPLE)

add_executable(010OpenGLBasicLighting "010 OpenGL Basic Lighting.cpp" "Shader.cpp" "Shader.h" ../../3rdparty/glad/src/gl.c
        ../../3rdparty/imgui/backends/imgui_impl_sdl3.cpp
        ../../3rdparty/imgui/backends/imgui_impl_sdl3.h
        ../../3rdparty/imgui/backends/imgui_impl_opengl3.cpp
        ../../3rdparty/imgui/backends/imgui_impl_opengl3.h
        InputHandler.cpp
        InputHandler.h
        Camera.cpp
        Camera.h
        Scancodes.h
)


target_include_directories(010OpenGLBasicLighting PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm ../../3rdparty/stb ../../3rdparty/imgui ../../3rdparty/imgui/backends )

target_link_libraries(010OpenGLBasicLighting PRIVATE SDL3::SDL3 imgui)

add_custom_command(TARGET 010OpenGLBasicLighting PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:010OpenGLBasicLighting>/assets/)

if(WIN32)
    add_custom_command(TARGET 010OpenGLBasicLighting PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:010OpenGLBasicLighting>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:010OpenGLBasicLighting>/)
endif()