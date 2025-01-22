cmake_minimum_required(VERSION 3.25)
project(004OpenGLShadersAttributes)

set(CMAKE_CXX_STANDARD 17)

add_executable(004OpenGLShadersAttributes "004 OpenGL Shaders Attributes.cpp" "Shader.h" "Shader.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(004OpenGLShadersAttributes PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(004OpenGLShadersAttributes PRIVATE SDL2::SDL2main SDL2::SDL2)