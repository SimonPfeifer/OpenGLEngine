#pragma once

#include "Material.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

/**
 * @brief Defines the types of shader.
 */
enum ShaderType
{
  vertex = 0,
  fragment = 1,
  geometry = 2,
};

/**
 * @brief Manages the construction and configuration of a shader program.
 */
class Shader
{
public:
	Shader();
  Shader(const char* vertPath, const char* fragPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	/**
	 * @brief Load and compile a shader program consisting of a vertex and 
	 * fragment shader.
	 * 
	 * @param vertPath Path to vertex shader.
	 * @param fragPath Path to fragment shader.
	 */
	void load(const char* vertPath, const char* fragPath);

	/**
	 * @brief Load and compiles the default shader for debugging.
	 */
	void loadDefault();

	/**
	 * @brief Loads a single type of shader. No compilation.
	 * 
	 * @param sourcePath Path to shader.
	 * @param type Type of shader.
	 */
	void loadShader(const char* sourcePath, const ShaderType type);

	/**
	 * @brief Compile a shader program with the currently loaded shaders.
	 */
	void compileShaderProgram();

	/**
	 * @brief Use/bind this shader program.
	 */
	void use() const;

	// TODO: This really needs a different solution. The shader and material do
	// 			 should probably not know about each other.
	/**
	 * @brief Bind the material properties to the appropriate shader points.
	 * 
	 * @param material Material to which to bind.
	 */
	void bindMaterial(const Material& material) const;
	
	/** Set shader uniform values. */
  void setUniformBool(const std::string& name, const bool value) const;
	void setUniformInt(const std::string& name, const int value) const;
	void setUniformFloat(const std::string& name, const float value) const;
  void setUniformVec3f(const std::string& name, const float value[]) const;
	void setUniformVec3f(const std::string& name, const glm::vec3 value) const;
	void setUniformVec4f(const std::string& name, const float value[]) const;
	void setUniformVec4f(const std::string& name, const glm::vec4 value) const;
	void setUniformMatrix4f(const std::string& name, const glm::mat4 value) const;

	/** Setters and getters. */
	GLuint getId() const {return m_shaderId;}

private:
	GLuint m_shaderId{};
	GLuint m_vertexShader{};
	GLuint m_fragmentShader{};

  GLuint m_geometryShader{};
  bool m_hasGeometryShader = false;

	/**
	 * @brief Load a type of shader and store it in the appriprtiate variable.
	 * 
	 * @param sourcePath Path to shader.
	 * @param glShaderType Type of shader.
	 * @param shader The shader variable in which to store the shader.
	 */
	void loadShaderType(const char* sourcePath, const GLenum glShaderType,
                      GLuint& shader);

	/**
	 * @brief Read the shader source file.
	 * 
	 * @param sourcePath Path to shader.
	 * @return std::string Shader source code.
	 */
	std::string readShaderSource(const char* sourcePath);

	/**
	 * @brief Get the shader compilation output including sucess flag and errors.
	 * 
	 * @param shader Shader program.
	 * @return int Compilation success state.
	 */
	int shaderCompileLog(GLuint shader);

	/**
	 * @brief Get the shader linking output including sucess flag and errors.
	 * 
	 * @param shader Shader program.
	 * @return int Linking success state.
	 */
  int shaderLinkLog(GLuint shader);
};
