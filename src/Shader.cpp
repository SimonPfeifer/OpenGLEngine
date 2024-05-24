#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader()
{
	shaderId = glCreateProgram();
}

Shader::Shader(const char* vertPath, const char* fragPath)
{
  shaderId = glCreateProgram();
  loadVertexShader(vertPath);
  loadFragmentShader(fragPath);
  compileShaderProgram();
}

Shader::~Shader()
{
	glDeleteProgram(shaderId);
}

void Shader::loadVertexShader(const char* sourcePath)
{
  std::string shaderSourceString = loadShaderSource(sourcePath);
  const char* shaderSource = shaderSourceString.c_str();
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &shaderSource, NULL);
  glCompileShader(vertexShader);
  if (!shaderCompileLog(vertexShader))
  {
    glDeleteShader(vertexShader);
    vertexShader = 0;
  };
}

void Shader::loadFragmentShader(const char* sourcePath)
{
  std::string shaderSourceString = loadShaderSource(sourcePath);
  const char* shaderSource = shaderSourceString.c_str(); 
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &shaderSource, NULL);
  glCompileShader(fragmentShader);
  if (!shaderCompileLog(fragmentShader))
  {
    glDeleteShader(fragmentShader);
    fragmentShader = 0;
  };
}

void Shader::compileShaderProgram()
{
  glAttachShader(shaderId, vertexShader);
  glAttachShader(shaderId, fragmentShader);
  glLinkProgram(shaderId);
  if (!shaderLinkLog(shaderId))
  {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    vertexShader = 0;
    fragmentShader = 0;
  };
}

void Shader::use()
{
  glUseProgram(shaderId);
}

void Shader::setUniformBool(const std::string& name, bool value)
{
  glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)value);
}

void Shader::setUniformInt(const std::string& name, int value)
{
  glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::setUniformFloat(const std::string& name, float value)
{
  glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::setUniformVec3f(const std::string& name, float value[])
{
  int shaderUniformLocation = glGetUniformLocation(shaderId, name.c_str());
  glUniform3fv(shaderUniformLocation, 1, value);
}

void Shader::setUniformVec3f(const std::string& name, glm::vec3 value)
{
  int shaderUniformLocation = glGetUniformLocation(shaderId, name.c_str());
  glUniform3fv(shaderUniformLocation, 1, glm::value_ptr(value));
}

void Shader::setUniformVec4f(const std::string& name, float value[])
{
  int shaderUniformLocation = glGetUniformLocation(shaderId, name.c_str());
  glUniform4fv(shaderUniformLocation, 1, value);
}

void Shader::setUniformVec4f(const std::string& name, glm::vec4 value)
{
  int shaderUniformLocation = glGetUniformLocation(shaderId, name.c_str());
  glUniform4fv(shaderUniformLocation, 1, glm::value_ptr(value));
}

void Shader::setUniformMatrix4f(const std::string& name, glm::mat4 value)
{
  int shaderUniformLocation = glGetUniformLocation(shaderId, name.c_str());
  glUniformMatrix4fv(shaderUniformLocation, 1, 0, glm::value_ptr(value));
}


std::string Shader::loadShaderSource(const char* sourcePath)
{
  std::string shaderSource;
  std::ifstream shaderFile;
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    shaderFile.open(sourcePath);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shaderSource = shaderStream.str();
  }
  catch (std::ifstream::failure const& e)
  {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  return shaderSource;
}

int Shader::shaderCompileLog(GLuint shader)
{
  int  shaderCompileSuccess;
  char shaderInfoLog[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompileSuccess);
  if (!shaderCompileSuccess)
  {
    glGetShaderInfoLog(shader, 1024, NULL, shaderInfoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << shaderInfoLog
      << std::endl;
  }
  return shaderCompileSuccess;
}

int Shader::shaderLinkLog(GLuint shader)
{
  int  shaderLinkSuccess;
  char shaderProgramLog[1024];
  glGetProgramiv(shader, GL_LINK_STATUS, &shaderLinkSuccess);
  if (!shaderLinkSuccess)
  {
    glGetProgramInfoLog(shader, 1024, NULL, shaderProgramLog);
    std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << shaderProgramLog
      << std::endl;
  }
  return shaderLinkSuccess;
}
