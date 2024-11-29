#include "Camera.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera()
{
	applyProjection();
  updateDirections();
  update();
}

Camera::Camera(const float fov, const float aspectRatio, const float zNear,
               const float zFar, const bool useOrtho) : Camera()
{
  setProjection(fov, aspectRatio, zNear, zFar, useOrtho);
}

void Camera::update()
{
  // TODO: Think about when to update the directions. Maybe have a "dirty" flag?
  updateDirections();
  m_view = glm::lookAt(position, position+m_frontDirection, m_upDirection);
}

void Camera::lookAt(const glm::vec3 target)
{
  glm::vec3 targetDir = glm::normalize(target - position);

  // Break the geometric degeneracy by checking which quadrant of yaw we are in,
  // i.e. if z is negative.
  pitch = asin(targetDir.y);
  yaw = asin(targetDir.x/cos(pitch));
  if(targetDir.z < 0.0f)
  {
    yaw = glm::pi<float>() - yaw;
  }
  
  pitch = glm::degrees(pitch);
  yaw = glm::degrees(yaw);

  updateDirections();
}

void Camera::setProjection(const float fov, const float aspectRatio,
                           const float zNear, const float zFar,
                           const bool useOrtho)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_zNear = zNear;
	m_zFar = zFar;
	m_useOrthographic = useOrtho;
  
  applyProjection();
}

void Camera::applyProjection()
{
	if (!m_useOrthographic)
	{
		m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio,
                                    m_zNear, m_zFar);
	}
	else
	{
		m_projection = glm::ortho(0.0f, m_orthoWidth, 0.0f, m_orthoWidth*m_fov,
                              m_zNear, m_zFar);
	}
}

void Camera::updateDirections()
{
  // Calculate the new coordinate frame of the camera using Euler angles.
  // Roll is not considered here.
  m_frontDirection.x = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  m_frontDirection.y = std::sin(glm::radians(pitch));
  m_frontDirection.z = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  m_frontDirection = glm::normalize(m_frontDirection);

  // Given the new forward direction vector, update the other directions.
  m_rightDirection = glm::normalize(glm::cross(m_frontDirection, m_worldUp));
  m_upDirection = glm::normalize(glm::cross(m_frontDirection, m_rightDirection));

  // Given OpenGL coordinate convention (+X,+Y,-Z) and the right hand rule for
  // the cross product calculation, the up direction points down. Fixed by
  // flipping the direction, i.e. multiplying by -1.
  m_upDirection *= -1.0f;
}

void Camera::applyKeyboardInput(const int keybordInput, const float deltaTime)
{
  float distance = m_movementSpeed * deltaTime;
  switch(keybordInput)
  {
  case 0:
    position += m_frontDirection * distance;
    break;
  case 1:
    position -= m_frontDirection * distance;
    break;
  case 2:
    position += m_rightDirection * distance;
    break;
  case 3:
    position -= m_rightDirection * distance;
    break;
  case 4:
    position += m_worldUp * distance;
    break;
  case 5:
    position -= m_worldUp * distance;
    break;
  default:
    std::cerr << "ERROR: invalid keybord input for camera: " << keybordInput
      << std::endl;
  }
}

void Camera::applyMouseInput(const float cursorPosX, const float cursorPosY,
                             const bool paused)
{
  // Pixel coordinate go from top left to bottom right. Therefore, inverse Y
  // direction to have the top of the screen be the positive Y direction.
  float cursorOffsetX = cursorPosX - m_lastCursorPosX;
  float cursorOffsetY = m_lastCursorPosY - cursorPosY;
  m_lastCursorPosX = cursorPosX;
  m_lastCursorPosY = cursorPosY;

  if (!paused)
  {
    cursorOffsetX *= m_sensitivity;
    cursorOffsetY *= m_sensitivity;

    // OpenGL coordinate frame (+X,+Y,-Z) means that up is positive pitch and
    // left is positive yaw when +Y is up, so we have to subtract the X offset.
    yaw -= cursorOffsetX;
    pitch += cursorOffsetY;

    // Clamp the pich between a negative and positive value less than 90 degrees.
    assert((m_maxPitchAngle < 90) &&
           "ERROR::applyMouseInput Max. pitch angle >= 90 degrees");
    pitch = std::max(-m_maxPitchAngle, std::min(m_maxPitchAngle, pitch));

    updateDirections();
  }
}
