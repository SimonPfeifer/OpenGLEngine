#pragma once

#include <glad/glad.h>


class FrameBuffer
{
public:
  FrameBuffer();

  void setDrawBuffer(GLenum buf);
  void setReadBuffer(GLenum buf);

  void bindTexture2D(GLenum attachment, GLuint texture);

  void checkStatus();

  GLuint getId() const {return bufferId;}

private:
  GLuint bufferId;
};