#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

/**
 * @brief Stores the data for a single vertex and is required by the Mesh class
 * to load geometry.
 */
struct Vertex
{
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 uv{};
};


/**
 * @brief The Mesh class loads, stores and manages geometry (positions, normals, 
 * texture coordinates, etc.) within OpenGL buffers. It is also resposible for
 * binding those buffers. Only indexed vertices are currently supported.
 */
class Mesh
{
public:
  Mesh();
  Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
  ~Mesh();

  /** There is currently no way to copy (or move) OpenGL buffers safely. */
  Mesh(const Mesh& other) = delete;
  Mesh& operator=(const Mesh& other) = delete;
  
  /** The ID assigned to a Mesh instance once stored in a Database */
  unsigned int id{};

  /**
   * @brief Creates a new VAO buffer containing vertex data and corresponding 
   * indices. Old data is deleted.
   * @param vertices The vertex data comprising the mesh.
   * @param indices The indices of the vertices that combine to make a face.
   */
  void loadVertexData(const std::vector<Vertex>& vertices,
                      const std::vector<int>& indices);

  /**
   * @brief  Binds the VAO of the current instance.
   */
  void bind() const;

  /**
   * @brief  Unbinds any currently bound VAO.
   */
  void unbind() const;

  /** Getters and setters. */
  int getNIndices() const {return m_nIndices;}
  GLuint getVao() const {return m_vao;}
  GLuint getVbo() const {return m_vbo;}
  GLuint getEbo() const {return m_ebo;}

private:
  int m_nIndices = 0;
  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};
};
