#include "Model.h"
#include "ObjLoader.h"

void Model::loadMesh(const char* filepath)
{
  ObjLoader loader;
  mesh = loader.load(filepath);
}

void Model::render(Camera camera, Light light, Shader &shader)
{
  for (size_t i=0; i<mesh.size(); ++i)
  {
    mesh[i].render(camera, light, shader);
  }
}