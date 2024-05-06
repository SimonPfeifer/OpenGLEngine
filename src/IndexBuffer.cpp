#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &iboId);
	count = 0;
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int dataCount)
	: count(dataCount)
{
	glGenBuffers(1, &iboId);
	loadIndexData(data, dataCount);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &iboId);
}

void IndexBuffer::loadIndexData(const unsigned int *data, unsigned int dataCount)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataCount * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
