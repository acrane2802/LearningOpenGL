cmake_minimum_required(VERSION 3.29)
project(LearningOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(3rdparty/sdl2)
add_executable(LearningOpenGL src/main.cpp 3rdparty/glad/src/glad.c)

target_include_directories(LearningOpenGL PRIVATE 3rdparty/sdl2/include 3rdparty/glad/include)

if(TARGET SDL2::SDL2main)
    target_link_libraries(LearningOpenGL PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(LearningOpenGL PRIVATE SDL2::SDL2)