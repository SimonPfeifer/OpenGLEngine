#pragma once

#include "Texture.h"

#include <glad/glad.h>

#include <string>

/**
 * @brief Manage a 2D texture resource.
 */
class Texture2D : public Texture
{
public:
	Texture2D();
  Texture2D(const char* filepath);

	bool loadTextureData(const char* filepath) override;
  bool loadTextureData(const std::string& filepath) override;

  void emptyTexture() override;
  void emptyTexture(const int width, const int height, const GLint format,
                    const GLenum type);

  /** Getters and setters. */
  int getWidth() const {return m_width;}
  int getHeight() const {return m_height;}

private:
  int m_width = 0;
  int m_height = 0;

  void setTextureData(const int width, const int height,
                      const GLint format, const GLenum type,
                      const unsigned char* data);
};
