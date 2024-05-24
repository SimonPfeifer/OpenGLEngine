#pragma once

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Shader.h"

#include <vector>

class Model
{
public:
  std::vector<Mesh> mesh;
  void loadMesh(const char* filepath);

  void render(Camera camera, Light light, Shader &shader);

private:

};