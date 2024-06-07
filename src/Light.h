#pragma once

#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>

class Light
{
public:
	Light();

  int type;
  bool enabled;

  // World space position.
  glm::vec3 position;
  glm::vec3 direction;

	float intensity;
	glm::vec3 color;

  float range;
  float spotlightAngle;

  void update(Camera& camera);

  glm::vec3 getPositionView() const {return positionView;}
  
private:
  // The position of the light in view space.
  glm::vec3 positionView;
};
