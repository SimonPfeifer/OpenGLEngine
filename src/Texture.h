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
	bool loadTextureData(const char* filepath);
  bool loadTextureData(std::string filepath);

  GLuint getId() const {return textureId;}

private:
	GLuint textureId;
};
