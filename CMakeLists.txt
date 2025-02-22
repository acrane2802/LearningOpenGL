cmake_minimum_required(VERSION 3.25)
project(LearningOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty/sdl)
add_subdirectory(3rdparty/glm)


#set(-DSDL_OPENGLES:BOOL=OFF -DOIS_BUILD_DEMOS:BOOL=OFF -DOIS_WIN32_XINPUT_SUPPORT:BOOL=OFF)

add_subdirectory(Examples/000OpenGLWindow)
add_subdirectory(Examples/001OpenGLTriangle)
add_subdirectory(Examples/001OpenGLTriangleExercises)
add_subdirectory(Examples/002OpenGLRectangle)
add_subdirectory(Examples/003OpenGLShaders)
add_subdirectory(Examples/004OpenGLShadersAttributes)
add_subdirectory(Examples/004OpenGLShadersAttributesExercises)
add_subdirectory(Examples/005OpenGLTextures)
add_subdirectory(Examples/005OpenGLTexturesExercises)
add_subdirectory(Examples/006OpenGLTransformations)
add_subdirectory(Examples/007OpenGLCoordinates)
add_subdirectory(Examples/008OpenGLCamera)