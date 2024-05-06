#include "Texture.h"
#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture()
{
	glGenTextures(1, &textureId);

	texWidth = 0;
	texHeight = 0;
	colourChannels = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

void Texture::loadTextureData(const char* filePath) 
{

	// Load GL texture buffer
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set texture mapping attributes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load the image data
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(filePath, &texWidth, &texHeight, 
		&colourChannels, 0);
	if (data)
	{
		// Bind image to GL texture buffer
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // automatically generate mipmap
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// Free image memory
	stbi_image_free(data);
}
