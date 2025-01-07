#pragma once

#include "Mesh.h"

class Quad : public Mesh
{
public:
  Quad();
  Quad(float width, float height);

  void generate(float width, float height);

private:

};

Quad::Quad(float width, float height)
{
  generate(width, height);
}

void Quad::generate(float width, float height)
{
  std::vector<Vertex> vertices;
  vertices.emplace_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f),
                        glm::vec2(0.0f, 0.0f)));
  
  vertices.emplace_back(Vertex(glm::vec3(width, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f),
                        glm::vec2(1.0f, 0.0f)));

  vertices.emplace_back(Vertex(glm::vec3(0.0f, height, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f),
                        glm::vec2(0.0f, 1.0f)));

  vertices.emplace_back(Vertex(glm::vec3(width, height, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f),
                        glm::vec2(1.0f, 1.0f)));

  std::vector<int> indices{0, 1, 2, 2, 1, 3};
  
  loadVertexData(vertices, indices);
}