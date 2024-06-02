#pragma once

#include "Asset.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light : public Asset
{
public:
	Light();

	float intensity;
	glm::vec4 color;

  void update(Camera& camera);
	void render(Camera& camera);

  glm::vec3 getPositionView() {return positionView;}
  
private:
  glm::vec3 positionView;
};
