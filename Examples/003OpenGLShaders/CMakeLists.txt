cmake_minimum_required(VERSION 3.25)
project(003OpenGLShaders)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

add_executable(003OpenGLShaders "003 OpenGL Shaders.cpp" ../../3rdparty/glad/src/gl.c)

target_include_directories(003OpenGLShaders PRIVATE ../../3rdparty/sdl/include ../../3rdparty/glad/include ../../3rdparty/glm)

target_link_libraries(003OpenGLShaders PRIVATE SDL3::SDL3)

add_custom_command(TARGET 003OpenGLShaders PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:003OpenGLShaders>/assets/)

if(WIN32)
    add_custom_command(TARGET 003OpenGLShaders PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:003OpenGLShaders>/../../3rdparty/sdl/SDL3.dll $<TARGET_FILE_DIR:003OpenGLShaders>/)
endif()