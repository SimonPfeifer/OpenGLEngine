#include "Light.h"

Light::Light()
{
  type = -1;
  enabled = true;

  direction = glm::vec3(0.0f);

	intensity = 0.0f;
  color = glm::vec3(1.0f);

  range = 0.0f;
  spotlightAngle = 0.0f;
	
  positionView = glm::vec3(0.0f);
}

void Light::update(Camera& camera)
{
  positionView = glm::vec3(camera.view * glm::vec4(position, 1.0f));
}
