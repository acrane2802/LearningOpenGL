cmake_minimum_required(VERSION 3.29)
project(000OpenGLWindow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(000OpenGLWindow "000 OpenGL Window.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(000OpenGLWindow PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(000OpenGLWindow PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2)