cmake_minimum_required(VERSION 3.25)
project(009OpenGLColors)

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

add_executable(009OpenGLColors "009 OpenGL Colors.cpp" "Shader.cpp" "Shader.h" ../../3rdparty/glad/src/gl.c
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


target_include_directories(009OpenGLColors PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm ../../3rdparty/stb ../../3rdparty/imgui ../../3rdparty/imgui/backends )

target_link_libraries(009OpenGLColors PRIVATE SDL3::SDL3 imgui)

add_custom_command(TARGET 009OpenGLColors PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:009OpenGLColors>/assets/)

if(WIN32)
    add_custom_command(TARGET 009OpenGLColors PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:009OpenGLColors>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:009OpenGLColors>/)
endif()