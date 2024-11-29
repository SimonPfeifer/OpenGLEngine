#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief The virtual camera responsible for tracking the view volume.
 * 
 * The camera has a position and orientation, as well as a view volume defined
 * by the field of view and the near and far planes. Also sets the projection
 * type.
 */
class Camera
{
public:
	Camera();
  Camera(const float fov, const float aspectRatio, const float zNear,
         const float zFar, const bool useOrtho);

  /** World position of the camera. */ 
	glm::vec3 position = glm::vec3(0.0f);

  /** Euler angles. */ 
  float yaw{};
  float pitch{};
  float roll{};

  /**
   * @brief Updates the view volume given its position and orientation.
   */
  void update();

  /**
   * @brief Updates the orientation to look at a target position.
   * 
   * @param target Target position.
   */
  void lookAt(const glm::vec3 target);

  /**
   * @brief Set the projection and bounds of the view volume.
   * 
   * @param fov The field of view.
   * @param aspectRatio Aspect ratio of width divided by height.
   * @param zNear The distance of the view volume near plane.
   * @param zFar The distance of the view volume far plane.
   * @param useOrtho Use orthographic projection; default is false (perspective).
   */
	void setProjection(const float fov, const float aspectRatio,
                     const float zNear, const float zFar,
                     const bool useOrtho = false);

  // TODO: These should be handled by an input handler and shouldn't be the
  //       responsibility of the camera.
  /**
   * @brief Moves the camera position based on keyboard input.
   * 
   * @param keybordInput Inputs can range from [0-5] corresponding to forward,
   * backward, right, left, up, down, respectively.
   * @param deltaTime Time elapsed since last update in seconds.
   */
  void applyKeyboardInput(const int keybordInput, const float deltaTime);
  
  /**
   * @brief Updates the camera orientation using the mouse curser coordinates.
   * 
   * @param cursorPosX Curser x-coordinate.
   * @param cursorPosY Curser y-coordinate.
   * @param paused Set to true if the orientation should not be updated.
   */
  void applyMouseInput(const float cursorPosX, const float cursorPosY,
                       const bool paused);

  /** Setter and getters. */
  glm::mat4 getViewMatrix() const {return m_view;}
  glm::mat4 getProjectionMatrix() const {return m_projection;}
  float getFOV() const {return m_fov;}
  void setFOV(float fov) {m_fov=fov; applyProjection();}
  float getAspectRatio() const {return m_aspectRatio;}
  void setAspectRatio(float aspectRatio) {m_aspectRatio=aspectRatio; applyProjection();}
  float getNear() const {return m_zNear;}
  void setNear(float near) {m_zNear=near; applyProjection();}
  float getFar() const {return m_zFar;}
  void setFar(float far) {m_zFar=far; applyProjection();}

private:
  /** The view and projection matrix. */
	glm::mat4 m_view{};
  glm::mat4 m_projection{};

  /** View volume quantities. */
	float m_fov = 45.0f;
	float m_aspectRatio = 16.0f / 9.0f;
	float m_zNear = 0.250f;
	float m_zFar = 250.0f;
	bool m_useOrthographic = false;
  float m_orthoWidth = 800.0f;

  /** Vectors tracking the camera coordinate frame, frontDirection is the
   * dirction the camera is facing. */
  glm::vec3 m_frontDirection = glm::vec3(0.0f);
  glm::vec3 m_rightDirection = glm::vec3(0.0f);
  glm::vec3 m_upDirection = glm::vec3(0.0f);

  /** Screen space cursor positions of the last frame. */
  float m_lastCursorPosX{};
  float m_lastCursorPosY{};

  /** This determines the world coordinate frame. */
  const glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

  /** Used to clamp the pitch value between negative and positive value. */
  float m_maxPitchAngle = 80.0f;

  /** The scalar factors to adjust the speed of mouse and keybord movement,
   * respectively. */
  float m_sensitivity = 0.08f;
	float m_movementSpeed = 30.0f;

  /**
   * @brief Updates the projection matrix.
   */
	void applyProjection();

  /**
   * @brief Updates the camera coordinate frame.
   */
  void updateDirections();
};
