cmake_minimum_required(VERSION 3.5)

project(main)

add_compile_options("$<$<CONFIG:Debug>:SHELL:-g -Wall -Wextra -Wpedantic -Werror>"
                   "$<$<CONFIG:Release>:SHELL:-O2 -DNDEBUG>" -std=c++20)

add_compile_options(-DGLFW_INCLUDE_NONE)

add_executable(main 
  main.cpp
  Camera.cpp
  FrameBuffer.cpp
  Material.cpp
  Mesh.cpp
  Scene.cpp
  ScreenQuad.cpp
  Shader.cpp
  Texture.cpp
  Transform.cpp
)

target_link_libraries(main 
  PRIVATE 
    ${OPENGL_LIBRARIES} 
    assimp::assimp 
    glfw 
    glm::glm 
    glad
    stb_image)
