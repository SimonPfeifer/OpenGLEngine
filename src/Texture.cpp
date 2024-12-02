#include "Texture.h"

#include "stb_image.h"

#include <iostream>

Texture::Texture() 
{
  glGenTextures(1, &textureId);
}

Texture::Texture(const char* filepath) : Texture()
{
  loadTextureData(filepath);
}

void Texture::bind(const int textureSlot) const
{
  glActiveTexture(GL_TEXTURE0 + textureSlot);
  glBindTexture(GL_TEXTURE_2D, textureId);
}


void Texture::emptyTexture2D(int width, int height,
                             GLint format, GLenum type)
{
  m_width = width;
  m_height = height;

  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, format,
               width, height, 0, format,
               type, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::emptyTexture3D(const int width, const int height, const int depth,
                             const GLint format, const GLenum type)
{
  m_width = width;
  m_height = height;
  m_depth = depth;

  glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format,
               width, height, depth, 0, format,
               type, NULL);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

bool Texture::loadTextureData(const char* filepath) 
{
  bool success;

	stbi_set_flip_vertically_on_load(true);

	// Load the image data.
  int colorChannels;
	unsigned char* data = stbi_load(filepath, &m_width, &m_height,
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

    // Load GL texture buffer.
    glBindTexture(GL_TEXTURE_2D, textureId);

		// Bind image to GL texture buffer.
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    
    // Set texture mapping attributes.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
    // Generate mipmap levels.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture.
    glBindTexture(GL_TEXTURE_2D, 0);

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

bool Texture::loadTextureData(const std::string filepath)
{
  return loadTextureData(filepath.c_str());
}

// TODO: Fix this madness!
// TODO: Make texture type part of the object.
void Texture::minMagFilter2D(const GLint minFilter, const GLint magFilter)
{

  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::wrapMode2D(const GLint sMode, const GLint tMode)
{
  // Warning! Hard wired for 2D texture!
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tMode);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::minMagFilter3D(const GLint minFilter, const GLint magFilter)
{
  // Warning! Hard wired for 3D texture!
  // TODO: Make texture type part of the object.
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture::wrapMode3D(const GLint sMode, const GLint tMode, const GLuint rMode)
{
  // Warning! Hard wired for 3D texture!
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, sMode);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, tMode);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, rMode);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}