cmake_minimum_required(VERSION 3.25)
project(001OpenGLTriangle)

set(CMAKE_CXX_STANDARD 17)

add_executable(001OpenGLTriangle "001 OpenGL Triangle.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(001OpenGLTriangle PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(001OpenGLTriangle PRIVATE SDL2::SDL2main SDL2::SDL2)