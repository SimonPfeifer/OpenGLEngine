#pragma once

#include "Asset.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light : public Asset
{
public:
	float intensity;
	glm::vec4 color;

	Light();

	void render(Camera camera);
};
