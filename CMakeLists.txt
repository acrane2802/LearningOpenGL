cmake_minimum_required(VERSION 3.29)
project(LearningOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(3rdparty/sdl2)
add_subdirectory(3rdparty/glm)
add_subdirectory(Examples/000OpenGLWindow)
add_subdirectory(Examples/001OpenGLTriangle)
add_subdirectory(Examples/002OpenGLRectangle)
