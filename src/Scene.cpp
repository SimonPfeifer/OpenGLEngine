#include "Scene.h"
#include "Texture2D.h"

#include <glad/glad.h>

#include <cassert>
#include <iostream>

// These should really be user defined per model import.
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices)


bool Scene::loadModel(const char* filepath,
                      glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filepath, ASSIMP_LOAD_FLAGS);

  bool success = false;
  if (scene)
  {
    success = loadModelComponents(scene, filepath, position, rotation, scale);
  }
  else
  {
    std::cerr << "ERROR::loadMesh Could not load file: " 
      << importer.GetErrorString() << std::endl;
  }

  return success;
}

void Scene::clear()
{
  instances.clear();
  meshes.clear();
  materials.clear();
  transforms.clear();
  lights.clear();
}

bool Scene::loadModelComponents(const aiScene* scene, const char* filepath,
  const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale)
{
  bool success = true;

  // Store the mesh and material IDs to match them for Instance creation.
  std::vector<unsigned int> meshIDs;
  std::vector<unsigned int> meshMaterialIndex;
  std::vector<unsigned int> materialIDs;

  // Process the mesh data.
  meshIDs.reserve(scene->mNumMeshes);
  meshMaterialIndex.reserve(scene->mNumMeshes);
  for (unsigned int i=0; i<scene->mNumMeshes; ++i)
  {
    // Should include error checking for mesh loading.
    loadMesh(scene->mMeshes[i], meshIDs, meshMaterialIndex);
  }

  // Process the material data.
  if (scene->HasMaterials())
  {
    materialIDs.reserve(scene->mNumMeshes);
    for (unsigned int i=0; i<scene->mNumMaterials; ++i)
    {
      if (!loadMaterial(scene->mMaterials[i], filepath, materialIDs))
      {
        success = false;
        std::cerr << "ERROR::loadMaterial Could not load material with index: "
          << i << std::endl;
      }
    }
  }

  // Create instance with mesh, material and transform.
  assert((meshIDs.size() == meshMaterialIndex.size()) &&
    "ERROR::processScene vector size mismatch");
  for (unsigned int i=0; i<meshIDs.size(); ++i)
  {
    Instance instance;
    instance.meshID = meshIDs[i];
    instance.materialID = materialIDs[meshMaterialIndex[i]];

    unsigned int transformID = transforms.add();
    
    transforms.get(transformID).setPosition(position);
    transforms.get(transformID).setRotation(rotation);
    transforms.get(transformID).setScale(scale);

    instance.transformIDs.push_back(transformID);

    instances.push_back(instance);
  }

  return success;
}

void Scene::loadMesh(const aiMesh* aiMesh, std::vector<unsigned int>& meshIDs,
                     std::vector<unsigned int>& meshMaterialIndex)
{
  // Load the vertex data.
  std::vector<Vertex> vertices;
  vertices.reserve(aiMesh->mNumVertices);
  aiVector3D zeroVec3(0.0f, 0.0f, 0.0f);
  for (unsigned int i=0 ; i<aiMesh->mNumVertices ; ++i) 
  {
    aiVector3D pos = aiMesh->mVertices[i];
    aiVector3D norm = (aiMesh->HasNormals() ? aiMesh->mNormals[i] : zeroVec3);

    // Assumed that there is only one texture coordinate set.
    aiVector3D uv = (aiMesh->HasTextureCoords(0) ? 
                    aiMesh->mTextureCoords[0][i] : zeroVec3);

    vertices.emplace_back(Vertex(glm::vec3(pos.x, pos.y, pos.z),
                                 glm::vec3(norm.x, norm.y, norm.z),
                                 glm::vec2(uv.x, uv.y)));
  }

  // Assemble the faces.
  std::vector<int> indices;
  indices.reserve(aiMesh->mNumFaces*3);
  for (unsigned int i=0; i<aiMesh->mNumFaces; ++i)
  {
    aiFace face = aiMesh->mFaces[i];
    assert((face.mNumIndices == 3) &&
      "ERROR::loadMesh Number of face vertices is not 3");

    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  // Copy new mesh to database and save ID for later Instance creation.
  unsigned int meshID = meshes.add();
  meshes.get(meshID).loadVertexData(vertices, indices);
  meshIDs.push_back(meshID);
  meshMaterialIndex.push_back(aiMesh->mMaterialIndex);
}

bool Scene::loadMaterial(const aiMaterial* aimaterial, const char* filepath,
                         std::vector<unsigned int>& materialIDs)
{
  bool success = true;

  Material material;

  aiString materialName;
  aimaterial->Get(AI_MATKEY_NAME, materialName);

  // Load constants.
  aiColor3D tempColor;
  if(aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, tempColor) != aiReturn_SUCCESS)
  {
    tempColor = {0.0f, 0.0f, 0.0f};
  }
  material.colorDiffuse = {tempColor.r, tempColor.g, tempColor.b};

  if(aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, tempColor) != aiReturn_SUCCESS)
  {
    tempColor = {0.0f, 0.0f, 0.0f};
  }
  material.colorSpecular = {tempColor.r, tempColor.g, tempColor.b};

  if(aimaterial->Get(AI_MATKEY_SHININESS, tempColor.r) != aiReturn_SUCCESS)
  {
    tempColor.r = 0.0f;
  }
  material.specularStrength = tempColor.r;

  if(aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, tempColor) != aiReturn_SUCCESS)
  {
    tempColor = {0.0f, 0.0f, 0.0f};
  }
  material.colorAmbient = {tempColor.r, tempColor.g, tempColor.b};

  // Load texture data.
  assert((filepath != NULL) && "ERROR::loadMaterial Input filepath is empty");

  // WARNING: This is a Windows specific path delimiter.
  const char delimitor = '\\';

  // Extract the directory part from the file path.
  std::string sfilepath = filepath;
  std::string::size_type lastSlashIndex = sfilepath.find_last_of(delimitor);
  std::string directory;

  if (lastSlashIndex == std::string::npos)
  {
    directory = "." + delimitor;
  }
  else if (lastSlashIndex == 0)
  {
    std::cerr << "ERROR::loadMateria does not support root path" << std::endl;
    success = false;
  }
  else
  {
    directory = sfilepath.substr(0, lastSlashIndex) + delimitor;
  }

  // Load each texture type.
  Texture2D texture;
  if (!loadMaterialTexture(aimaterial, aiTextureType_DIFFUSE, directory, texture))
  {
    success = false;
    std::cerr << "ERROR::loadMaterial "
      << "Could not load texture type DIFFUSE." << std::endl;
  }
  material.textureDiffuse = texture;

  if (!loadMaterialTexture(aimaterial, aiTextureType_SPECULAR, directory, texture))
  {
    success = false;
    std::cerr << "Scene::loadMaterial "
      << "Could not load texture type SPECULAR." << std::endl;
  }
  material.textureSpecular = texture;

  // Copy new material to database and save ID for later Instance creation.
  unsigned int materialID = materials.add();
  materials.get(materialID) = material;
  materialIDs.push_back(materialID);

  return success;
}

bool Scene::loadMaterialTexture(const aiMaterial* aimaterial,
                                const enum aiTextureType textureType,
                                const std::string filepath,
                                Texture& outTexture)
{
  aiString localTexturePath;
  bool hasTexture = aimaterial->GetTexture(textureType, 0, &localTexturePath,
    NULL, NULL, NULL, NULL, NULL)  == aiReturn_SUCCESS;
  
  // Only use the first texture even if there are more.
  if (aimaterial->GetTextureCount(textureType) > 0 && hasTexture)
  {    
    return outTexture.loadTextureData(filepath+localTexturePath.data);
  }
  return true;
}
