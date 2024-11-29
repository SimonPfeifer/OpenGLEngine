#pragma once

#include <glad/glad.h>

#include <string>

/**
 * @brief Manage a texture resource.
 */
class Texture
{
public:
	Texture();
  Texture(const char* filepath);

  /**
   * @brief Bind the rexture to a texture slot.
   * 
   * @param textureSlot Number of the texture slot.
   */
  void bind(const int textureSlot) const;

  /**
   * @brief Generate an empty 2D texture object.
   * 
   * @param width Texture width.
   * @param height Texture height.
   * @param format OpenGL texture format.
   * @param type OpenGL texture type.
   */
  void emptyTexture2D(const int width, const int height, const GLint format,
                      const GLenum type);

    /**
   * @brief Generate an empty 3D texture object.
   * 
   * @param width Texture width.
   * @param height Texture height.
   * @param depth Texture depth.
   * @param format OpenGL texture format.
   * @param type OpenGL texture type.
   */
  void emptyTexture3D(const int width, const int height, const int depth,
                      const GLint format, const GLenum type);

  /**
   * @brief Load a texture image. Return success state.
   * 
   * @param filepath Path to image.
   * @return true 
   * @return false 
   */
	bool loadTextureData(const char* filepath);
  bool loadTextureData(const std::string filepath);

  /**
   * @brief Set the minify and magnify filtering for a 2D texture.
   * 
   * @param minFilter OpenGL filter type for minify.
   * @param magFilter OpenGL filter type for magnify.
   */
  void minMagFilter2D(const GLint minFilter, const GLint magFilter);

  /**
   * @brief Set the wrapping behaviour at the edges of a 2D texture.
   * 
   * @param sMode OpenGL mode in the s direction.
   * @param tMode OpenGL mode in the t direction.
   */
  void wrapMode2D(const GLint sMode, const GLint tMode);

  /**
   * @brief Set the minify and magnify filtering for a 3D texture.
   * 
   * @param minFilter OpenGL filter type for minify.
   * @param magFilter OpenGL filter type for magnify.
   */
  void minMagFilter3D(const GLint minFilter, const GLint magFilter);

  /**
   * @brief Set the wrapping behaviour at the edges of a 3D texture.
   * 
   * @param sMode OpenGL mode in the s direction.
   * @param tMode OpenGL mode in the t direction.
   * @param rMode OpenGL mode in the r direction.
   */
  void wrapMode3D(const GLint sMode, GLint tMode, const GLuint rMode);

  /** Getters and setters. */
  GLuint getId() const {return textureId;}
  int getWidth() const {return m_width;}
  int getHeight() const {return m_height;}
  int getDepth() const {return m_depth;}

private:
	GLuint textureId{};

	int m_width{};
	int m_height{};
  int m_depth{};
};
