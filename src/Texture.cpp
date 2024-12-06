#include "Texture.h"

#include <iostream>

Texture::Texture(const GLenum textureType, const GLint textureFormat,
                 const GLenum textureDataType)
  : m_textureType{textureType}, m_textureFormat{textureFormat},
    m_textureDataType{textureDataType}
{
  glGenTextures(1, &m_textureId);
}

void Texture::bind() const
{
  glBindTexture(m_textureType, m_textureId);
}
void Texture::unbind() const
{
  glBindTexture(m_textureType, 0);
}

void Texture::bindToSlot(const int textureSlot) const
{
  glActiveTexture(GL_TEXTURE0 + textureSlot);
  glBindTexture(m_textureType, m_textureId);
}

void Texture::minMagFilter(const GLint minFilter, const GLint magFilter)
{
  glBindTexture(m_textureType, m_textureId);
  glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, magFilter);
  glBindTexture(m_textureType, 0);
}

void Texture::wrapMode(const GLint sMode, const GLint tMode, const GLint rMode)
{
  glBindTexture(m_textureType, m_textureId);
  glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, sMode);
  glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, tMode);
  glTexParameteri(m_textureType, GL_TEXTURE_WRAP_R, rMode);
  glBindTexture(m_textureType, 0);
}

void Texture::generateMipmap() const
{
  glBindTexture(m_textureType, m_textureId);
  glGenerateMipmap(m_textureType);
  glBindTexture(m_textureType, 0);
}

GLenum Texture::translateTextureFormat(const int nColorChannels) const
{
  switch(nColorChannels)
  {
  case 1:
    return GL_RED;
  case 3:
    return GL_RGB;
  case 4:
    return GL_RGBA;
  default:
    std::cerr << "ERROR::translateTextureFormat unknown texture color format."
      << std::endl;
    return GL_FALSE; // This is an invalid format.
  }
}
