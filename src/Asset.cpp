#include "Asset.h"
#include <iostream>

Asset::Asset()
{
	glGenVertexArrays(1, &vaoId);
	vertexLength = 0;
	indexLength = 0;
	
  scale = glm::vec3(1.0f);
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);

	model = glm::mat4(1.0f);
}

Asset::~Asset()
{
	GLint vboId;
	GLint nAttr = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
	glBindVertexArray(vaoId);
	for (int i = 0; i < nAttr; i++)
	{
		vboId = 0;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
		if (vboId > 0)
		{
			glDeleteBuffers(1, (GLuint*) & vboId);
		}
	}
}

void Asset::loadVertexData(float data[], unsigned int dataLength,
	unsigned int index[], unsigned int indexLength, unsigned int stride,
	unsigned int attributeIndex)
{
	// Set the length of the index data for the first time.
	// Make sure any new data matches the length of the previous index.
	if (this->indexLength > 0)
	{
		if (indexLength != this->indexLength)
			std::cout << "WARNING::ASSET::vertex index data mismatch " <<
			this->indexLength <<" " << indexLength << std::endl;
	} else
	{
		this->indexLength = indexLength;
	}

	// Bind this VAO.
	glBindVertexArray(vaoId);

	// Generate VBO and load the vertex data.
	GLuint vboId;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, stride*dataLength *sizeof(float), data, GL_STATIC_DRAW);

	// Generate EBO and load the vertices index data.
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength *sizeof(unsigned int), index, GL_STATIC_DRAW);

	// Set the layout of the data and bind to VAO.
	glVertexAttribPointer(attributeIndex, stride, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)0);
	glEnableVertexAttribArray(attributeIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	
	// Unbind vertex array .
	glBindVertexArray(0);
}

void Asset::render(Camera camera)
{
	applyTransformation();

	shader.use();
	shader.setUniformMatrix4f("model", model);
  shader.setUniformMatrix4f("view", camera.view);
  shader.setUniformMatrix4f("projection", camera.projection);

	glBindTexture(GL_TEXTURE_2D, texture.textureId);
	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Asset::setScale(float newScale)
{
	scale = glm::vec3(newScale);
}

void Asset::setScale(glm::vec3 newScale)
{
	scale = newScale;
}

void Asset::applyScale()
{
	model = glm::scale(model, scale);
}

void Asset::applyTranslation()
{
	model = glm::translate(model, position);
}

void Asset::applyRotation()
{
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Asset::applyTransformation()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
}

void Asset::resetTransformation()
{
	model = glm::mat4(1.0f);
}
