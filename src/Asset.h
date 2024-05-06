#pragma once

#include "Camera.h"
#include "shader/Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Asset
{
public:
	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 rotation;

	Shader shader;
	Texture texture;

	glm::mat4 model;

	GLuint vaoId;
	unsigned int vertexLength;
	unsigned int indexLength;

public:
	Asset();
	~Asset();

	void render(Camera camera);

	void setScale(float scale);
	void setScale(glm::vec3 scale);
	void applyScale();
	void applyTranslation();
	void applyRotation();
	void applyTransformation();
	void resetTransformation();

	void loadVertexData(float vertexData[], unsigned int vertexDataLength,
		unsigned int indexData[], unsigned int indexDataLength, unsigned int stride,
		unsigned int attributeIndex);
};