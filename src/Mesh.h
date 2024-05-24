#pragma once

#include "Camera.h"
#include "Light.h"
#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>
#include <string> 

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

class Mesh
{
public:
  Mesh(std::vector<Vertex> vertices,
       std::vector<int> indices);

  Mesh(std::vector<glm::vec3> vert, std::vector<int> indices);

  void render(Camera camera, Light light, Shader &shader);

  std::vector<Vertex> vertices;
  std::vector<int> indices;

private:

  GLuint vbo;
  GLuint ebo;
  GLuint vao;

  void loadMesh();
};