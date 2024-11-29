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
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Instance
{
  unsigned int meshID;
  unsigned int materialID;
  std::vector<unsigned int> transformIDs;
};

// TODO: Maybe keep Scene as an aggragate and move functions into a SceneManager.
/**
 * @brief The scene stores all of the objects required for rendering. It is
 * primarily a data class with some functionally to load the data.
 */
class Scene
{
public:
  Scene() = default;
  ~Scene();

  Scene(const Scene& other) = delete;
  Scene& operator=(const Scene& other) = delete;

  Camera camera{};

  std::vector<Light> lights;
  
  Database<Mesh> meshes{};
  Database<Material> materials{};
  Database<Transform> transforms{};
  std::vector<Instance> instances;
  
  /**
   * @brief Load a model from disk. Will separate composite models into
   * constituents. Uses the assimp library to read the model files and is
   * therefore compatible with a range of file types. Returns whether the model
   * was sucessfully imported.
   * 
   * @param filepath Path to model file.
   * @param position Position transform to be applied to the model.
   * @param rotation Rotational transform to be applied to the model.
   * @param scale Scaling to be applied to the model.
   * @return true 
   * @return false 
   */
  bool loadModel(const char* filepath,
                 glm::vec3 position = glm::vec3(0.0f),
                 glm::vec3 rotation = glm::vec3(0.0f),
                 glm::vec3 scale = glm::vec3(1.0f));

  /**
   * @brief Removes all data from the internal storage.
   */
  void clear();

private:
  /**
   * @brief Loads model componenets given an assimp scene object. Returns the
   * success of the import.
   * 
   * @param scene Assimp scene object.
   * @param filepath Path to the model file.
   * @param position Position transform to be applied to the model.
   * @param rotation Rotational transform to be applied to the model.
   * @param scale Scaling to be applied to the model.
   * @return true 
   * @return false 
   */
  bool loadModelComponents(const aiScene* scene, const char* filepath,
                           const glm::vec3 position, const glm::vec3 rotation,
                           const glm::vec3 scale);

  /**
   * @brief Loads a single mesh from an assimp mesh object. Input vector are 
   * used to match the mesh to their respective materials to create an instance.
   * 
   * @param aiMesh Assimp mesh object.
   * @param meshIDs Vector of mesh IDs.
   * @param meshMaterialIndex Vector of IDs used to match meshes to materials.
   */
  void loadMesh(const aiMesh* aiMesh, std::vector<unsigned int>& meshIDs,
                std::vector<unsigned int>& meshMaterialIndex);

  /**
   * @brief Loads a single material from an assimp material object.
   * 
   * @param aimaterial Assimp material object.
   * @param filepath Path to the model file.
   * @param materialIDs Vector of material IDs.
   * @return true 
   * @return false 
   */
  bool loadMaterial(const aiMaterial* aimaterial, const char* filepath,
                    std::vector<unsigned int>& materialIDs);

  /**
   * @brief Loads a single texture from an assimp material object.
   * 
   * @param aimaterial Assimp material object.
   * @param textureType Type of texture.
   * @param filepath Path to the model file.
   * @param outTexture Output texture object.
   * @return true 
   * @return false 
   */
  bool loadMaterialTexture(const aiMaterial* aimaterial,
                           const enum aiTextureType textureType,
                           const std::string filepath,
                           Texture& outTexture);
};
