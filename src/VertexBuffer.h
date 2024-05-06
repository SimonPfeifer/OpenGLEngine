#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class VertexBuffer
{
public:
	GLuint vboId;

	VertexBuffer();
	VertexBuffer(const void* data, unsigned int dataLength);
	~VertexBuffer();

	void loadVertexData(const void* data, unsigned int dataLength);

	void bind() const;
	void unbind() const;
};
