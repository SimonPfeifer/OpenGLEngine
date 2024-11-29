#pragma once

#include "glm/glm.hpp"

/**
 * @brief Stores the transform of an objects instance and calculates the model
 * matrix.
 */
class Transform
{
public:
  Transform() = default;
  ~Transform() = default;

  /** The ID assigned to a Mesh instance once stored in a Database */
  unsigned int id{};

  /** Getters and setters. */
  glm::mat4 getModelTransform();
  glm::vec3 getPosition() const {return m_position;}
  void setPosition(glm::vec3 position) {m_position=position; m_isDirty=true;}
  glm::vec3 getRotation() const {return m_rotation;}
  void setRotation(glm::vec3 rotation) {m_rotation=rotation; m_isDirty=true;}
  glm::vec3 getScale() const {return m_scale;}
  void setScale(glm::vec3 scale) {m_scale=scale; m_isDirty=true;}
  
private:
  /** A flag that keeps track if any changes have been made to the transform. */
  bool m_isDirty = true;

  /** Transform and model matrix. */
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
  glm::mat4 m_model = glm::mat4(1.0f);
};
