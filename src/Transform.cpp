#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::getModelTransform()
{
	// Only recalculate model matrix if any of the transform properties have
	// changed.
	if (!m_isDirty)
	{
		return m_model;
	}

  m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_position);
	m_model = glm::rotate(m_model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_model = glm::rotate(m_model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_model = glm::rotate(m_model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_model = glm::scale(m_model, m_scale);
	m_isDirty = false;
	
  return m_model;
}
