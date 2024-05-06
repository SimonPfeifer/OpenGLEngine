#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class IndexBuffer
{
public:
	GLuint iboId;
	GLuint count;

	IndexBuffer();
	IndexBuffer(const unsigned int *data, unsigned int dataCount);
	~IndexBuffer();

	void loadIndexData(const unsigned int *data, unsigned int dataCount);

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; };
};
