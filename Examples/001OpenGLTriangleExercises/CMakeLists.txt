cmake_minimum_required(VERSION 3.25)
project(001OpenGLTriangleExercises)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(001OpenGLTriangleExercises "001 OpenGL Triangle Exercises.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(001OpenGLTriangleExercises PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(001OpenGLTriangleExercises PRIVATE SDL2::SDL2main SDL2::SDL2)