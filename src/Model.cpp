#include "Model.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <iostream>

// These should be really be user defined per model import.
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

Model::Model()
{
  scale = glm::vec3(1.0f);
  position = glm::vec3(0.0f);
  rotation = glm::vec3(0.0f);

  model = glm::mat4(1.0f);
}

Model::Model(const char* filepath) : Model()
{
  loadModel(filepath);
}

bool Model::loadModel(const char* filepath)
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

void Model::update()
{
  applyTransformations();
}

void Model::render(Camera& camera, Light& light, Shader& shader)
{
  // Load variables into shader.
  shader.use();
  shader.setUniformMatrix4f("modelView", camera.view * model);
  shader.setUniformMatrix4f("projection", camera.projection);

  shader.setUniformVec3f("cameraPosition", camera.position);
  shader.setUniformVec3f("lightPosition", light.getPositionView());
  shader.setUniformVec4f("lightColor", light.color);

  for (size_t i=0; i<meshes.size(); ++i)
  {
    // Load material data.
    if (i==0 || materialIndices[i]!=materialIndices[i-1])
      materials[materialIndices[i]].activate(shader);

    // Render mesh.
    meshes[i].draw();
  }
  glBindVertexArray(0);
}

void Model::applyTransformations()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
}

void Model::clear()
{
  meshes.clear();
  materialIndices.clear();
  materials.clear();
}

bool Model::processScene(const aiScene* scene, const char* filepath)
{
  bool success = true;

  // Process the mesh data.
  for (int i=0; i<scene->mNumMeshes; ++i)
  {
    // Should include error checking for mesh loading.
    loadMesh(scene->mMeshes[i]);
  }

  // Process the material data.
  if (scene->HasMaterials())
  {
    for (int i=0; i<scene->mNumMaterials; ++i)
    {
      if (!loadMaterial(scene->mMaterials[i], filepath))
      {
        success = false;
        std::cerr << "ERROR::loadMaterial Could not load material with index: "
          << i << std::endl;
      }
    }
  }

  return success;
}

void Model::loadMesh(const aiMesh* aiMesh)
{
  Vertex vertex;
  std::vector<Vertex> vertices;
  std::vector<int> indices;

  // Load the vertex data.
  aiVector3D zeroVec3(0.0f, 0.0f, 0.0f);
  for (unsigned int i=0 ; i<aiMesh->mNumVertices ; ++i) 
  {
    aiVector3D pos = aiMesh->mVertices[i];
    aiVector3D norm = (aiMesh->HasNormals() ? aiMesh->mNormals[i] : zeroVec3);

    // Assumed that there is only one texture coordinate set.
    aiVector3D uv = (aiMesh->HasTextureCoords(0) ? 
                    aiMesh->mTextureCoords[0][i] : zeroVec3);

    vertex.position = glm::vec3(pos.x, pos.y, pos.z);
    vertex.normal = glm::vec3(norm.x, norm.y, norm.z);
    vertex.uv = glm::vec2(uv.x, uv.y);
    vertices.push_back(vertex);
  }

  // Assemble the faces.
  for (unsigned int i=0; i<aiMesh->mNumFaces; ++i)
  {
    aiFace face = aiMesh->mFaces[i];
    assert(face.mNumIndices == 3);

    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  meshes.push_back(Mesh(vertices, indices));
  materialIndices.push_back(aiMesh->mMaterialIndex);
}

bool Model::loadMaterial(const aiMaterial* aimaterial, const char* filepath)
{
  bool success = true;

  Material material;

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
  Texture texture;
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
    std::cerr << "Model::loadMaterial "
      << "Could not load texture type SPECULAR." << std::endl;
  }
  material.textureSpecular = texture;

  materials.push_back(material);

  return success;
}

bool Model::loadMaterialTexture(const aiMaterial* aimaterial,
                                enum aiTextureType textureType,
                                std::string directory,
                                Texture& outTexture)
{
  aiString localTexturePath;
  bool hasTexture = aimaterial->GetTexture(textureType, 0, &localTexturePath,
    NULL, NULL, NULL, NULL, NULL)  == aiReturn_SUCCESS;
  
  // Only use the first texture even if there are more.
  if (aimaterial->GetTextureCount(textureType) > 0 && hasTexture)
  {    
    return outTexture.loadTextureData(directory+localTexturePath.data);
  }
  return true;
}

