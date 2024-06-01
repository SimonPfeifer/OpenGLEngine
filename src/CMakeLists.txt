cmake_minimum_required(VERSION 3.5)

project(main)

add_compile_options("$<$<CONFIG:Debug>:SHELL:-Wall -Wextra -Wpedantic -Werror>"
                   "$<$<CONFIG:Release>:SHELL:-O2 -DNDEBUG>" -std=c++20)

add_compile_options(-DGLFW_INCLUDE_NONE)

add_executable(main 
  main.cpp
  Asset.cpp
  AssetBox.cpp
  Camera.cpp
  Light.cpp
  LightBox.cpp
  Material.cpp
  Mesh.cpp
  Model.cpp
  Shader.cpp
  Texture.cpp
)

target_link_libraries(main 
  PRIVATE 
    ${OPENGL_LIBRARIES} 
    assimp::assimp 
    glfw 
    glm::glm 
    glad
    stb_image)
