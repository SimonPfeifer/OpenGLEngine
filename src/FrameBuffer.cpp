#include <iostream>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
  glGenFramebuffers(1, &bufferId);
}

void FrameBuffer::setDrawBuffer(GLenum buf)
{
  glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
  glDrawBuffer(buf);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setReadBuffer(GLenum buf)
{
  glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
  glReadBuffer(buf);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture2D(GLenum attachment, GLuint texture)
{
  glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         attachment,
                         GL_TEXTURE_2D,
                         texture,
                         0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture3D(GLenum attachment, GLuint texture)
{
  glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
  glFramebufferTexture(GL_FRAMEBUFFER,
                       attachment,
                       texture,
                       0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::checkStatus()
{
  // Check if the framebuffer was setup correctly.
  glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cerr << "MAIN::Frame buffer "<< bufferId << " failed: " <<
    status << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}