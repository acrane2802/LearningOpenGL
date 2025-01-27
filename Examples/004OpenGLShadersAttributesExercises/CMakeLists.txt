cmake_minimum_required(VERSION 3.25)
project(004OpenGLShadersAttributesExercises)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(004OpenGLShadersAttributesExercises "004 OpenGL Shaders Attributes Exercises.cpp" "Shader.h" "Shader.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(004OpenGLShadersAttributesExercises PRIVATE ../../3rdparty/sdl2/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(004OpenGLShadersAttributesExercises PRIVATE SDL2::SDL2main SDL2::SDL2)

add_custom_command(TARGET 004OpenGLShadersAttributesExercises PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:004OpenGLShadersAttributesExercises>/assets/)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(WIN32)
        add_custom_command(TARGET 004OpenGLShadersAttributesExercises PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:004OpenGLShadersAttributesExercises>/../../3rdparty/sdl2/SDL2d.dll $<TARGET_FILE_DIR:004OpenGLShadersAttributesExercises>/)
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        add_custom_command(TARGET 004OpenGLShadersAttributesExercises PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE_DIR:004OpenGLShadersAttributesExercises>/../../3rdparty/sdl2/SDL2.dll $<TARGET_FILE_DIR:004OpenGLShadersAttributesExercises>/)
    endif()
endif()