#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


class Shader
{
public:
	GLuint shaderId;
	GLuint vertexShader;
	GLuint fragmentShader;

	Shader();
  Shader(const char* vertPath, const char* fragPath);
	~Shader();
	void loadVertexShader(const char* sourcePath);
	void loadFragmentShader(const char* sourcePath);
	void compileShaderProgram();
	void use();
	void setUniformBool(const std::string& name, bool value);
	void setUniformInt(const std::string& name, int value);
	void setUniformFloat(const std::string& name, float value);
  void setUniformVec3f(const std::string& name, float value[]);
	void setUniformVec3f(const std::string& name, glm::vec3 value);
	void setUniformVec4f(const std::string& name, float value[]);
	void setUniformVec4f(const std::string& name, glm::vec4 value);
	void setUniformMatrix4f(const std::string& name, glm::mat4 value);

private:
	std::string loadShaderSource(const char* sourcePath);
	int shaderCompileLog(GLuint shader);
  int shaderLinkLog(GLuint shader);
};
