#pragma once

#include "Camera.h"
#include "Database.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <vector>

struct Instance
{
  unsigned int meshID;
  unsigned int materialID;
  std::vector<unsigned int> transformIDs;
};

// TODO: Maybe keep Scene as an aggragate and move methods into a SceneManager.
class Scene
{
public:
  Camera camera;

  std::vector<Instance> instances;

  Database<Mesh> meshes;
  Database<Material> materials;
  Database<Transform> transforms;
  
  std::vector<Light> lights;
  
  bool loadModel(const char* filepath);

  void clear();

private:
  bool processScene(const aiScene* scene, const char* filepath);
  void loadMesh(const aiMesh* aiMesh, std::vector<unsigned int>& meshIDs,
                std::vector<unsigned int>& meshMaterialIndex);
  bool loadMaterial(const aiMaterial* aimaterial, const char* filepath,
                    std::vector<unsigned int>& materialIDs);
  bool loadMaterialTexture(const aiMaterial* aimaterial,
                           enum aiTextureType textureType,
                           std::string directory,
                           Texture& outTexture);
};
