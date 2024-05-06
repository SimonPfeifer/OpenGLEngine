#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &vboId);
}

VertexBuffer::VertexBuffer(const void* data, unsigned int dataLength)
{
	glGenBuffers(1, &vboId);
	loadVertexData(data, dataLength);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vboId);
}

void VertexBuffer::loadVertexData(const void* data, unsigned int dataLength)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, 3 * dataLength * sizeof(float), data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
