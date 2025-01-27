cmake_minimum_required(VERSION 3.25)
project(000OpenGLWindow)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(000OpenGLWindow "000 OpenGL Window.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(000OpenGLWindow PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(000OpenGLWindow PRIVATE SDL2::SDL2main SDL2::SDL2)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(WIN32)
        add_custom_command(TARGET 000OpenGLWindow PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:000OpenGLWindow>/../../3rdparty/sdl2/SDL2d.dll $<TARGET_FILE_DIR:000OpenGLWindow>/)
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        add_custom_command(TARGET 000OpenGLWindow PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:000OpenGLWindow>/../../3rdparty/sdl2/SDL2.dll $<TARGET_FILE_DIR:000OpenGLWindow>/)
    endif()
endif()