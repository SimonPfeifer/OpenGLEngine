#pragma once

#include <glad/glad.h>

#include <string>

class Texture
{
public:

	int width;
	int height;
	
	Texture();
  Texture(const char* filepath);

  void bind(const int textureSlot) const;

  void emptyTexture2D(int width, int height, GLint format, GLenum type);

	bool loadTextureData(const char* filepath);
  bool loadTextureData(std::string filepath);

  void minMagFilter(GLint minFilter, GLint magFilter);
  void wrapMode(GLint sMode, GLint tMode);

  GLuint getId() const {return textureId;}

private:
	GLuint textureId;
};
