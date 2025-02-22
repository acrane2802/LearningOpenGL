cmake_minimum_required(VERSION 3.25)
project(000OpenGLWindow)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(000OpenGLWindow "000 OpenGL Window.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(000OpenGLWindow PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(000OpenGLWindow PRIVATE SDL3::SDL3)

if(WIN32)
    add_custom_command(TARGET 000OpenGLWindow PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:000OpenGLWindow>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:000OpenGLWindow>/)
endif()