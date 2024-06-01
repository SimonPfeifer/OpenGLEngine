#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

class Mesh
{
public:
  Mesh(std::vector<Vertex> vertices, std::vector<int> indices);

  void draw();

private:
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  int nIndices;
};
