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
  Mesh();
  Mesh(std::vector<Vertex> vertices, std::vector<int> indices);
  
  // Database ID.
  unsigned int id;

  GLuint vao;
  int nIndices;

private:
  GLuint vbo;
  GLuint ebo;
};
