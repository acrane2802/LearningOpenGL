cmake_minimum_required(VERSION 3.29)
project(002OpenGLRectangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(002OpenGLRectangle "002 OpenGL Rectangle.cpp" ../../3rdparty/glad/src/glad.c)

target_include_directories(002OpenGLRectangle PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(002OpenGLRectangle PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2)