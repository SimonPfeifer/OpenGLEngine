#include "Mesh.h"

#include <glad/glad.h>

#include <cstddef>
#include <iostream>
#include <string>

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<int> indices)
{
  this->vertices = vertices;
  this->indices = indices;

  loadMesh();
}

Mesh::Mesh(std::vector<glm::vec3> vert, std::vector<int> indices)
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // Bind this VAO.
	glBindVertexArray(vao);

	// Generate VBO and load the vertex data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3*vert.size()*sizeof(float), vert.data(), GL_STATIC_DRAW);

	// Generate EBO and load the vertices index data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Set the layout of the data and bind to VAO.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	// Unbind vertex array .
	glBindVertexArray(0);
}

void Mesh::loadMesh()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // Load data into vertex array object.
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               &vertices[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
               &indices[0], GL_STATIC_DRAW);

  // Set the layout of the data and bind to VAO.
	// Positions.
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Normals.
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  // Texture coordinates.
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	
  // Unbind vertex array .
  glBindVertexArray(0);
}

void Mesh::render(Camera camera, Light light, Shader &shader)
{
  // Load variables into shader.
 	shader.use();
	shader.setUniformMatrix4f("model", glm::mat4(1.0f));
  shader.setUniformMatrix4f("view", camera.view);
  shader.setUniformMatrix4f("projection", camera.projection);

  shader.setUniformVec3f("lightPosition", light.position);

  // // Load textures into shader.
  // for (int i=0; i < textures.size(); i++)
  // {
  //   glActiveTexture(GL_TEXTURE0 + i);
  //   glBindTexture(GL_TEXTURE_2D, textures[i].id);
  //   shader.setUniformInt("texture"+std::to_string(i), i);
  // }
  // glActiveTexture(GL_TEXTURE0);

  // Render the mesh.
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); 
}
