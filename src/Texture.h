#pragma once

#include <glad/glad.h>

#include <string>

class Texture
{
public:

	int width;
	int height;
  int depth;
	
	Texture();
  Texture(const char* filepath);

  void bind(const int textureSlot) const;

  void emptyTexture2D(int width, int height, GLint format, GLenum type);
  void emptyTexture3D(int width, int height, int depth,
                      GLint format, GLenum type);

	bool loadTextureData(const char* filepath);
  bool loadTextureData(std::string filepath);

  void minMagFilter2D(GLint minFilter, GLint magFilter);
  void wrapMode2D(GLint sMode, GLint tMode);
  void minMagFilter3D(GLint minFilter, GLint magFilter);
  void wrapMode3D(GLint sMode, GLint tMode);

  GLuint getId() const {return textureId;}

private:
	GLuint textureId;
};
