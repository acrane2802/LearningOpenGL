cmake_minimum_required(VERSION 3.25)
project(006OpenGLTransformations)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(006OpenGLTransformations "006 OpenGL Transformations.cpp" "Shader.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(006OpenGLTransformations PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm ../../3rdparty/stb)

target_link_libraries(006OpenGLTransformations PRIVATE SDL3::SDL3)

add_custom_command(TARGET 006OpenGLTransformations PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:006OpenGLTransformations>/assets/)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(WIN32)
        add_custom_command(TARGET 006OpenGLTransformations PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:006OpenGLTransformations>/../../3rdparty/sdl2/SDL2d.dll $<TARGET_FILE_DIR:006OpenGLTransformations>/)
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        add_custom_command(TARGET 006OpenGLTransformations PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:006OpenGLTransformations>/../../3rdparty/sdl2/SDL2.dll $<TARGET_FILE_DIR:006OpenGLTransformations>/)
    endif()
endif()