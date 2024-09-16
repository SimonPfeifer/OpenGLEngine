#include "Mesh.h"
#include "ScreenQuad.h"

ScreenQuad::ScreenQuad()
{
  xmin = 0.0f;
  xmax = 0.0f;
  ymin = 0.0f;
  ymax = 0.0f;
}
ScreenQuad::ScreenQuad(float xmin, float xmax, float ymin, float ymax)
{
  this->xmin = xmin;
  this->xmax = xmax;
  this->ymin = ymin;
  this->ymax = ymax;
  generate(xmin, xmax, ymin, ymax);
}

void ScreenQuad::generate(float xmin, float xmax, float ymin, float ymax)
{
  std::vector<Vertex> vertices;
  vertices.push_back(Vertex(glm::vec3(xmin, ymin, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec2(0.0f, 0.0f)));
  
  vertices.push_back(Vertex(glm::vec3(xmax, ymin, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec2(1.0f, 0.0f)));

  vertices.push_back(Vertex(glm::vec3(xmin, ymax, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec2(0.0f, 1.0f)));

  vertices.push_back(Vertex(glm::vec3(ymax, ymax, 0.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec2(1.0f, 1.0f)));

  std::vector<int> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(1);
  indices.push_back(3);

  loadVertexData(vertices, indices);
}