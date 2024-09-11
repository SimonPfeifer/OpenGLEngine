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

  //--------------------------- (0 bytes)
  glm::vec3 colorGlobalAmbient;
  glm::vec3 colorAmbient;
  glm::vec3 colorDiffuse;
  glm::vec3 colorSpecular;
  glm::vec3 colorEmissive;

  glm::vec4 reflectance;

  float specularStrength;
  float opacity;

  float indexOfRefraction;

  bool hasAmbientTexture;
  bool hasDiffuseTexture;
  bool hasSpecularTexture;
  bool hasSpecularPowerTexture;
  bool hasEmissiveTexture;

  bool hasNormalTexture;
  bool hasBumpTexture;
  bool hasOpacityTexture;
  float bumpIntensity;

  float specularScale;
  float alphaThreshold;
  glm::vec2  padding;
  //--------------------------- (??? bytes)

  Texture textureDiffuse;
  Texture textureSpecular;

  
  void activate(Shader& shader) const;
  
private:

};
