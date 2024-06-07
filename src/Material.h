#pragma once

#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <vector>

class Material
{
public:
  Material();

  // Database ID.
  unsigned int id;

  glm::vec3 colorAmbient;
  glm::vec3 colorDiffuse;
  glm::vec3 colorSpecular;
  float specularStrength;

  Texture textureDiffuse;
  Texture textureSpecular;

  void activate(Shader& shader) const;
  
private:

};
