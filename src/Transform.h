#pragma once

#include "glm/glm.hpp"

class Transform
{
public:
  Transform();

  // Database ID.
  unsigned int id;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

  glm::mat4 model;

  glm::mat4 getModelTransform();
};
