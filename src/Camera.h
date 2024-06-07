#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
  Camera(float fov, float aspectRatio, float zNear, float zFar, bool useOrtho);

  // World position of the camera.
	glm::vec3 position;

  // The view and projection matrix.
	glm::mat4 view;
  glm::mat4 projection;

	void setProjection(float fov, float aspectRatio, float zNear, float zFar, bool useOrtho);

  void applyKeybordInput(int keybordInput, float deltaTime);
  void applyMouseInput(float cursorPosX, float cursorPosY, bool paused);

  void updateView();

private:
  // Projection quantities.
	float fov;
	float aspectRatio;
	float zNear;
	float zFar;
	bool useOrthographic;

  // Vectors tracking the wold coordinate frame. frontDirection is the
  // dirction the camera is facing.
  glm::vec3 frontDirection;
  glm::vec3 rightDirection;
  glm::vec3 upDirection;

  // Euler angles
  float yaw;
  float pitch;
  float roll;

  // Screen space cursor positions of the last frame.
  float lastCursorPosX;
  float lastCursorPosY;

  // This determines the world coordinate frame.
  glm::vec3 worldUp;

  // Used to clamp the pitch value between negative and positive value.
  float maxPitchAngle;

  // The scalar factors to adjust the speed of mouse and keybord movement,
  // respectively.
  float sensitivity;
	float movementSpeed;

	void applyProjection();
  void updateDirections();
};
