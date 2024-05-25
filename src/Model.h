#pragma once

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class Model
{
public:
  bool loadMesh(const char* filepath);
  void clear();

  void render(Camera camera, Light light, Shader &shader);

private:
  std::vector<Mesh> meshes;
  std::vector<int> materialIndices;

  bool processScene(const aiScene* scene, const char* filepath);
  bool initMesh(const aiMesh* aiMesh);
  bool initMaterials(const aiScene* scene, const char* filepath);
};