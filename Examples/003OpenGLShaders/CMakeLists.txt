cmake_minimum_required(VERSION 3.29)
project(003OpenGLShaders)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(003OpenGLShaders "003 OpenGL Shaders.cpp" ../../3rdparty/glad/src/glad.c)

target_include_directories(003OpenGLShaders PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(003OpenGLShaders PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2)