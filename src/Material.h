#pragma once

#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <vector>

class Material
{
public:
  glm::vec3 colorDiffuse;
  glm::vec3 colorSpecular;
  float specularStrength;
  glm::vec3 colorAmbient;

  Texture textureDiffuse;
  Texture textureSpecular;

  void activate(Shader& shader);
  
private:

};
