# Good example of a CMake project.
# https://stackoverflow.com/questions/13703647/how-to-properly-add-include-directories-with-cmake

cmake_minimum_required(VERSION 3.5)

project(
    OpenGLEngine
    VERSION 0.1
    DESCRIPTION "Toy OpenGL rendering engine.")


set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Main source files.
add_subdirectory(src)

# External libraries.
add_subdirectory(external/assimp)
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/stb_image)
