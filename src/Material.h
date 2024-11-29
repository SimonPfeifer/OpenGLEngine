#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

#include <vector>

/**
 * @brief Defines the properties of a material along with the material textures.
 */
struct Material
{
public:
  Material() = default;
  ~Material() = default;

  // Database ID.
  unsigned int id{};

  // Size of POD important for packing in an OpenGL buffer.
  // 0 bytes
  glm::vec3 colorGlobalAmbient{};
  glm::vec3 colorAmbient{};
  glm::vec3 colorDiffuse{};
  glm::vec3 colorSpecular{};
  glm::vec3 colorEmissive{};
  // 36 bytes

  glm::vec4 reflectance{};
  // 52 bytes

  float specularStrength{};
  float opacity{};
  // 60 bytes

  float indexOfRefraction{};
  // 64 bytes

  bool hasAmbientTexture{};
  bool hasDiffuseTexture{};
  bool hasSpecularTexture{};
  bool hasSpecularPowerTexture{};
  bool hasEmissiveTexture{};
  // 69 bytes

  bool hasNormalTexture = false;
  bool hasBumpTexture = false;
  bool hasOpacityTexture = false;
  float bumpIntensity{};
  // 76 bytes

  float specularScale{};
  float alphaThreshold{};
  glm::vec2 padding{};
  // 92 bytes

  Texture textureDiffuse{};
  Texture textureSpecular{};
};
