#pragma once

#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>

/**
 * @brief Defines the type of light.
 */
enum LightType
{
  point = 0,
  spot = 1,
  directional = 2,
};

/**
 * @brief Defines the properties of a light objects.
 */
struct Light
{
  glm::vec3 position = glm::vec3(0.0f);
  float range = 10.0f;
  // 16 bytes

  glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
  float spotlightAngle = 90.0f;
  // 16 bytes

  glm::vec3 color = glm::vec3(0.0f);
  float intensity = 100.0f;
  // 16 bytes

  // int enabled = 1;
  int type = LightType::point;
  glm::vec3 dummy = glm::vec3(0.0f);
  // 16 bytes
}; // total 64 bytes
