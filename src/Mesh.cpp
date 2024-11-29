#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh()
{
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices)
  : Mesh()
{
  loadVertexData(vertices, indices);
}

Mesh::~Mesh()
{
  glDeleteBuffers(1, &m_ebo);
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_vao);
}

void Mesh::loadVertexData(const std::vector<Vertex>& vertices,
                          const std::vector<int>& indices)
{
  m_nIndices = indices.size();

  bind();

  // Reserve space in the buffers.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               &vertices[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
               &indices[0], GL_STATIC_DRAW);

  // Fill buffers with data.
	// Positions.
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Normals.
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, normal));

  // Texture coordinates.
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, uv));

  unbind();
}

void Mesh::bind() const
{
  glBindVertexArray(m_vao);
}

void Mesh::unbind() const
{
  // Since OpenGL is a big state machine, this technically unbinds any VAO
  // currently bound; not just the current instance.
  glBindVertexArray(0);
}
