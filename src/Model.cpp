#include "Model.h"

#include <glm/glm.hpp>

#include <iostream>

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

void Model::clear()
{
  meshes.clear();
  materialIndices.clear();
}

bool Model::loadMesh(const char* filepath)
{
  clear();

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filepath, ASSIMP_LOAD_FLAGS);

  bool success = false;
  if (scene)
  {
    success = processScene(scene, filepath);
  }
  else
  {
    std::cerr << "ERROR::loadMesh Could not load file: " 
      << importer.GetErrorString() << std::endl;
  }

  return success;
}


bool Model::processScene(const aiScene* scene, const char* filepath)
{
  bool success = false;

  // Process all the meshes.
  for (unsigned int i=0; i<scene->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[i];
    initMesh(mesh);
  }

  // Process all the materials.
  success = initMaterials(scene, filepath);

  return success;
}

bool Model::initMesh(const aiMesh* aiMesh)
{
  Vertex vertex;
  std::vector<Vertex> vertices;
  std::vector<int> indices;

  aiVector3D zeroVec3(0.0f, 0.0f, 0.0f);
  for (unsigned int i=0 ; i<aiMesh->mNumVertices ; ++i) 
  {
    aiVector3D pos = aiMesh->mVertices[i];
    aiVector3D norm = (aiMesh->HasNormals() ? aiMesh->mNormals[i] : zeroVec3);

    // Assumed that there is only one texture coordinate set.
    aiVector3D uv = (aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0][i] : zeroVec3);

    vertex.position = glm::vec3(pos.x, pos.y, pos.z);
    vertex.normal = glm::vec3(norm.x, norm.y, norm.z);
    vertex.uv = glm::vec2(uv.x, uv.y);
    vertices.push_back(vertex);
  }

  for (unsigned int i=0; i<aiMesh->mNumFaces; ++i)
  {
    aiFace face = aiMesh->mFaces[i];
    assert(face.mNumIndices == 3);

    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  meshes.push_back(Mesh(vertices, indices));

  return true;
}

bool Model::initMaterials(const aiScene* scene, const char* filepath)
{
  std::cout << scene << filepath << std::endl;
  return true;
}


void Model::render(Camera camera, Light light, Shader &shader)
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
  for (size_t i=0; i<meshes.size(); ++i)
  {
    glBindVertexArray(meshes[i].vao);
    glDrawElements(GL_TRIANGLES, meshes[i].nIndices, GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}