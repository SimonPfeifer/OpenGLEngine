cmake_minimum_required(VERSION 3.5)

project(
    OpenGLEngine
    VERSION 0.1
    DESCRIPTION "Toy OpenGL rendering engine.")

add_subdirectory(src)
add_subdirectory(external/assimp)
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
