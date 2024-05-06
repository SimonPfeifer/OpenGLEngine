#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture
{
public:
	int texWidth;
	int texHeight;
	int colourChannels;
	unsigned int textureId;
	
	Texture();
	~Texture();

	void loadTextureData(const char* filePath);
};
