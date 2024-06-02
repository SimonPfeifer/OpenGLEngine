#include "Light.h"

Light::Light()
{
	intensity = 1.0f;
	color = glm::vec4(1.0f);

  positionView = glm::vec3(0.0f);
}

void Light::update(Camera& camera)
{
  positionView = glm::vec3(camera.view * glm::vec4(position, 1.0f));
}

void Light::render(Camera& camera)
{
  glUseProgram(shader.shaderId);

	applyTransformation();
	shader.setUniformMatrix4f("model", model);
  shader.setUniformMatrix4f("view", camera.view);
  shader.setUniformMatrix4f("projection", camera.projection);

	shader.setUniformVec4f("lightColor", color);

	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
