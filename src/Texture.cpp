#include "Texture.h"

#include "stb_image.h"

#include <iostream>

Texture::Texture()
{
	width = 0;
	height = 0;
}

Texture::Texture(const char* filepath) : Texture()
{
  loadTextureData(filepath);
}


void Texture::bind(const int textureSlot)
{
  glActiveTexture(GL_TEXTURE0 + textureSlot);
  glBindTexture(GL_TEXTURE_2D, textureId);
}

bool Texture::loadTextureData(const char* filepath) 
{
  bool success;

	stbi_set_flip_vertically_on_load(true);

	// Load the image data.
  int colorChannels;
	unsigned char* data = stbi_load(filepath, &width, &height,
		&colorChannels, 0);
	if (data)
	{
    GLenum format;
    if (colorChannels == 1)
      format = GL_RED;
    else if (colorChannels == 3)
      format = GL_RGB;
    else if (colorChannels == 4)
      format = GL_RGBA;
    else
      std::cerr << "ERROR::loadTextureData unknown texture color format."
        << std::endl;

    // Generate texture buffer.
    glGenTextures(1, &textureId);

    // Load GL texture buffer.
    glBindTexture(GL_TEXTURE_2D, textureId);

		// Bind image to GL texture buffer.
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    
    // Set texture mapping attributes.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
    // Generate mipmap levels.
    glGenerateMipmap(GL_TEXTURE_2D);

    success = true;
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
    success = false;
	}
	// Free image memory.
	stbi_image_free(data);

  return success;
}

bool Texture::loadTextureData(std::string filepath)
{
  return loadTextureData(filepath.c_str());
}
