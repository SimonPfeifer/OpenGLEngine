#pragma once

#include <string>

#include <GLFW/glfw3.h>

/**
 * @brief Defines set resolutions.
 */
enum class WindowResolution
{
  r480,
  r720,
  r1080,
};

/**
 * @brief Manages a window instance.
 */
class Window
{
public:
  Window();
  Window(const std::string& title, int width, int height, bool fullscreen=false);
  ~Window();

  Window(const Window& other) = delete;
  Window& operator=(const Window&) = delete;
  
  /**
   * @brief Close the window.
   */
  void close();

  /**
   * @brief Check if window is closing.
   * 
   * @return int Returns true if the window is still active.
   */
  int isAlive();

  /**
   * @brief The OpenGL instance is double buffered by default. This swaps the 
   * frame buffers.
   * 
   */
  void swapBuffers() const;

  /** Getters and setters. */
  GLFWwindow* getWindowInstance() const {return m_window;}
  
  void getSize(int& width, int& height) const;
  void setSize(const int width, const int height);
  void setResolution(const WindowResolution resolution);

  void setFullscreen() const;
  void setWindowed() const;
  bool isFullscreen() const;

private:
  GLFWwindow* m_window{};

  std::string m_title;
  int m_width{};
  int m_height{};
  int m_xposition = 0;
  int m_yposition = 0;
};
