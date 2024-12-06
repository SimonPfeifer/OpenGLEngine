#pragma once

#include <glad/glad.h>

#include <string>

/**
 * @brief Manage a texture resource.
 */
class Texture
{
public:
  Texture(const GLenum textureType, const GLint textureFormat,
          const GLenum textureDataType);
	virtual ~Texture() = default;
  
  /**
   * @brief Bind the texture.
   * 
   * @param textureSlot Number of the texture slot.
   */
  virtual void bind() const;

  /**
   * @brief Unbind the texture.
   * 
   * @param textureSlot Number of the texture slot.
   */
  virtual void unbind() const;

  /**
   * @brief Bind the texture to a texture slot.
   * 
   * @param textureSlot Number of the texture slot.
   */
  virtual void bindToSlot(const int textureSlot) const;

  /**
   * @brief Load a texture image. Return success state.
   * 
   * @param filepath Path to image.
   * @return true 
   * @return false 
   */
	virtual bool loadTextureData(const char* filepath) = 0;
  virtual bool loadTextureData(const std::string& filepath) = 0;

  /**
   * @brief Empty the data in the texture.
   */
  virtual void emptyTexture() = 0;

  /**
   * @brief Set the minify and magnify filtering.
   * 
   * @param minFilter OpenGL filter type for minify.
   * @param magFilter OpenGL filter type for magnify.
   */
  virtual void minMagFilter(const GLint minFilter, const GLint magFilter);
  
  /**
   * @brief Set the wrapping behaviour at the edges of the texture.
   * 
   * @param sMode OpenGL mode in the S direction.
   * @param tMode OpenGL mode in the T direction.
   * @param rMode OpenGL mode in the R direction.
   */
  virtual void wrapMode(const GLint sMode, const GLint tMode, const GLint rMode);

  /**
   * @brief Generate a complete mipmap set for this texture.
   */
  virtual void generateMipmap() const;

  /** Getters and setters. */
  GLuint getId() const {return m_textureId;}
  GLenum getTextureType() const {return m_textureType;}
  GLint getTextureFormat() const {return m_textureFormat;}
  GLuint getTextureDataType() const {return m_textureDataType;}

protected:
	GLuint m_textureId{};
  GLenum m_textureType{};
  GLint m_textureFormat{};
  GLenum m_textureDataType{};

  GLenum translateTextureFormat(const int nColorChannels) const;
};
