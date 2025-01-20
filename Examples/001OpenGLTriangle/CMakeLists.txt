cmake_minimum_required(VERSION 3.29)
project(001OpenGLTriangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(001OpenGLTriangle "001 OpenGL Triangle.cpp" ../../3rdparty/glad/src/glad.c)

target_include_directories(001OpenGLTriangle PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(001OpenGLTriangle PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2)