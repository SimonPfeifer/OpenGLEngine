#pragma once

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>

class Model
{
public:
  Model();
  Model(const char* filepath);
  
  bool loadModel(const char* filepath);

	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 rotation;

  void update();

  void render(Camera& camera, Light& light, Shader& shader);

private:
  // Model transformations.
  glm::mat4 model;

  void applyTransformations();
  
  // Mesh and material loading.
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
  std::vector<unsigned int> materialIndices;
  
  void clear();
  bool processScene(const aiScene* scene, const char* filepath);
  void loadMesh(const aiMesh* aiMesh);
  bool loadMaterial(const aiMaterial* aimaterial, const char* filepath);
  bool loadMaterialTexture(const aiMaterial* aimaterial,
                           enum aiTextureType textureType,
                           std::string directory,
                           Texture& outTexture);
};
