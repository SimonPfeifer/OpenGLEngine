#include "Camera.h"

#include <iostream>
#include <math.h>

#include <iostream>

Camera::Camera()
{
  position = glm::vec3(0.0f);

	fov = 45.0f;
	aspectRatio = 16.0f / 9.0f;
	zNear = 1.0f;
	zFar = 300.0f;
  useOrthographic = false;
  projection = glm::mat4(1.0f);
	applyProjection();
	
  frontDirection = glm::vec3(0.0f);
  rightDirection = glm::vec3(0.0f);
  upDirection = glm::vec3(0.0f);
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

  yaw = 0.0f;
  pitch = 0.0f;
  roll = 0.0f;
  updateDirections();

	view = glm::mat4(1.0f);
  update();

  lastCursorPosX = 0.0f;
  lastCursorPosY = 0.0f;

  maxPitchAngle = 80.0f;

  sensitivity = 0.08f;
  movementSpeed = 30.0f;
}

Camera::Camera(float fov, float aspectRatio, float zNear, float zFar, bool useOrtho) : Camera()
{
  setProjection(fov, aspectRatio, zNear, zFar, useOrtho);
}

void Camera::update()
{
  view = glm::lookAt(position, position+frontDirection, upDirection);
}

void Camera::lookAt(glm::vec3 target)
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
}

void Camera::setProjection(float fov, float aspectRatio, float zNear, float zFar, bool useOrtho)
{
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->zNear = zNear;
	this->zFar = zFar;
	this->useOrthographic = useOrtho;
  
  applyProjection();
}

void Camera::applyProjection()
{
	if (!useOrthographic)
	{
		projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
	}
	else
	{
		float width = 800.0f;
		projection = glm::ortho(0.0f, width, 0.0f, width*fov, zNear, zFar);
	}
}

void Camera::updateDirections()
{
  // Calculate the new coordinate frame of the camera using Euler angles.
  // Roll is not considered here.
  frontDirection.x = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  frontDirection.y = std::sin(glm::radians(pitch));
  frontDirection.z = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  frontDirection = glm::normalize(frontDirection);

  // Given the new forward direction vector, update the other directions.
  rightDirection = glm::normalize(glm::cross(frontDirection, worldUp));
  upDirection = glm::normalize(glm::cross(frontDirection, rightDirection));

  // Given OpenGL coordinate convention (+X,+Y,-Z) and the right hand rule for
  // the cross product calculation, the up direction points down. Fixed by
  // flipping the direction, i.e. multiplying by -1.
  upDirection *= -1.0f;
}

void Camera::applyKeybordInput(int keybordInput, float deltaTime)
{
  float speed = movementSpeed * deltaTime;
  if (keybordInput == 0)
    position += frontDirection * speed;
  else if (keybordInput == 1)
    position += -frontDirection * speed;
  else if (keybordInput == 2)
    position += rightDirection * speed;
  else if (keybordInput == 3)
    position += -rightDirection * speed;
  else if (keybordInput == 4)
    position += worldUp * speed;
  else if (keybordInput == 5)
    position += -worldUp * speed;
  else
    std::cerr << "ERROR: invalid keybord input for camera: " << keybordInput
      << std::endl;
}

void Camera::applyMouseInput(float cursorPosX, float cursorPosY, bool paused)
{
  // Pixel coordinate go from top left to bottom right. Therefore, inverse Y
  // direction to have the top of the screen be the positive Y direction.
  float cursorOffsetX = cursorPosX - lastCursorPosX;
  float cursorOffsetY = lastCursorPosY - cursorPosY;
  lastCursorPosX = cursorPosX;
  lastCursorPosY = cursorPosY;

  if (!paused)
  {
    cursorOffsetX *= sensitivity;
    cursorOffsetY *= sensitivity;

    // OpenGL coordinate frame (+X,+Y,-Z) means that up is positive pitch and
    // left is positive yaw when +Y is up, so we have to subtract the X offset.
    yaw -= cursorOffsetX;
    pitch += cursorOffsetY;

    // Clamp the pich between a negative and positive value less than 90 degrees.
    pitch = std::max(-maxPitchAngle, std::min(maxPitchAngle, pitch));

    updateDirections();
  }
}
