#include "ImageLoader.h"
#include "Texture2D.h"

#include <iostream>

Texture2D::Texture2D() 
  : Texture(GL_TEXTURE_2D, GL_RGB, GL_FLOAT)
{
  // Empty constructor.
}

Texture2D::Texture2D(const char* filepath)
  : Texture2D()
{
  loadTextureData(filepath);
}

bool Texture2D::loadTextureData(const char* filepath) 
{
  ImageLoader loader;
  int nChannels{};
  if (loader.load(filepath, m_width, m_height, nChannels))
	{
    m_textureFormat = translateTextureFormat(nChannels);

		// Bind image to GL texture buffer.
    setTextureData(m_width, m_height, m_textureFormat, GL_UNSIGNED_BYTE,
                   loader.getImageData());
    
    // Set texture mapping attributes.
    wrapMode(GL_REPEAT, GL_REPEAT, GL_REPEAT);
    minMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Generate mipmap levels.
    generateMipmap();

    return true;
	}
  return false;
}

bool Texture2D::loadTextureData(const std::string& filepath)
{
  return loadTextureData(filepath.c_str());
}

void Texture2D::emptyTexture()
{
  glBindTexture(m_textureType, m_textureId);
  setTextureData(m_width, m_height, m_textureFormat, m_textureDataType, NULL);
  glBindTexture(m_textureType, 0);
}

void Texture2D::emptyTexture(const int width, const int height, GLint format,
                             GLenum type)
{
  glBindTexture(m_textureType, m_textureId);
  setTextureData(width, height, format, type, NULL);
  glBindTexture(m_textureType, 0);
}

void Texture2D::setTextureData(const int width, const int height,
                               const GLint format, const GLenum type,
                               const unsigned char* data)
{
  glBindTexture(m_textureType, m_textureId);
  glTexImage2D(m_textureType, 0, format,
               width, height, 0, format,
               type, data);
  m_width = width;
  m_height = height;
  glBindTexture(m_textureType, 0);
}