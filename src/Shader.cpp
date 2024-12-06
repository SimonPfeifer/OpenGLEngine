#include "Material.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


Shader::Shader()
  : m_shaderId{glCreateProgram()}
{

}

Shader::Shader(const char* vertPath, const char* fragPath) : Shader()
{
  load(vertPath, fragPath);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderId);
}

void Shader::load(const char* vertPath, const char* fragPath)
{
  loadShader(vertPath, ShaderType::vertex);
  loadShader(fragPath, ShaderType::fragment);
  compileShaderProgram();
}

void Shader::loadDefault()
{
  load("..\\..\\res\\shaders\\default.vert",
       "..\\..\\res\\shaders\\default.frag");
}

void Shader::loadShader(const char* sourcePath, const ShaderType type)
{
  switch(type)
  {
  case ShaderType::vertex:
    loadShaderType(sourcePath, GL_VERTEX_SHADER, m_vertexShader);
    break;
  case ShaderType::fragment:
    loadShaderType(sourcePath, GL_FRAGMENT_SHADER, m_fragmentShader);
    break;
  case ShaderType::geometry:
    loadShaderType(sourcePath, GL_GEOMETRY_SHADER, m_geometryShader);
    break;
  default:
    std::cerr << "ERROR::SHADER::Unknown shader type." << std::endl;
    return;
  }
}

void Shader::compileShaderProgram()
{
  glAttachShader(m_shaderId, m_vertexShader);
  glAttachShader(m_shaderId, m_fragmentShader);

  if (m_hasGeometryShader)
    glAttachShader(m_shaderId, m_geometryShader);
  
  glLinkProgram(m_shaderId);
  if (!shaderLinkLog(m_shaderId))
  {
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    m_vertexShader = 0;
    m_fragmentShader = 0;

    if (m_hasGeometryShader)
    {
      glDeleteShader(m_geometryShader);
      m_geometryShader = 0;
      m_hasGeometryShader = false;
    }
  };
}

void Shader::use() const
{
  glUseProgram(m_shaderId);
}

void Shader::bindMaterial(const Material& material) const
{
  // Warning: This assumes implicit knowledge between *all* shaders and *all*
  //          materials. One way to ensure this relationship is to have a
  //          universal material in which case the binding is necessary only 
  //          once. Could be done when during use(). 
  // TODO: Build a universal material and shader layout.
  use();

  // Set the material uniforms.
  setUniformVec3f("colorAmbient", material.colorAmbient);
  setUniformVec3f("colorDiffuse", material.colorDiffuse);
  setUniformVec3f("colorSpecular", material.colorSpecular);
  setUniformFloat("specularStrength", material.specularStrength);
  
  // Bind the textures.
  setUniformInt("textureDiffuse", 0);
  material.textureDiffuse.bindToSlot(0);

  setUniformInt("textureSpecular", 1);
  material.textureSpecular.bindToSlot(1);
}

void Shader::setUniformBool(const std::string& name, const bool value) const
{
  glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), static_cast<int>(value));
}

void Shader::setUniformInt(const std::string& name, const int value) const
{
  glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setUniformFloat(const std::string& name, const float value) const
{
  glUniform1f(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setUniformVec3f(const std::string& name, const float value[]) const
{
  int shaderUniformLocation = glGetUniformLocation(m_shaderId, name.c_str());
  glUniform3fv(shaderUniformLocation, 1, value);
}

void Shader::setUniformVec3f(const std::string& name, const glm::vec3 value) const
{
  int shaderUniformLocation = glGetUniformLocation(m_shaderId, name.c_str());
  glUniform3fv(shaderUniformLocation, 1, glm::value_ptr(value));
}

void Shader::setUniformVec4f(const std::string& name, const float value[]) const
{
  int shaderUniformLocation = glGetUniformLocation(m_shaderId, name.c_str());
  glUniform4fv(shaderUniformLocation, 1, value);
}

void Shader::setUniformVec4f(const std::string& name, const glm::vec4 value) const
{
  int shaderUniformLocation = glGetUniformLocation(m_shaderId, name.c_str());
  glUniform4fv(shaderUniformLocation, 1, glm::value_ptr(value));
}

void Shader::setUniformMatrix4f(const std::string& name, const glm::mat4 value) const
{
  int shaderUniformLocation = glGetUniformLocation(m_shaderId, name.c_str());
  glUniformMatrix4fv(shaderUniformLocation, 1, 0, glm::value_ptr(value));
}

void Shader::loadShaderType(const char* sourcePath, const GLenum glShaderType,
                            GLuint& shader)
{
  std::string shaderSourceString = readShaderSource(sourcePath);
  const char* shaderSource = shaderSourceString.c_str(); 
  
  shader = glCreateShader(glShaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  if (!shaderCompileLog(shader))
  {
    glDeleteShader(shader);
    shader = 0;
  }
}

std::string Shader::readShaderSource(const char* sourcePath)
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
