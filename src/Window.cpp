#include <iostream>

#include "Constants.h"
#include "Window.h"


void window_size_callback(GLFWwindow*, int width, int height)
{
  glViewport(0, 0, width, height);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
  glViewport(0, 0, width, height);
}

void window_pos_callback(GLFWwindow*, int xpos, int ypos)
{
  // Add callback functionality.
}

void window_close_callback(GLFWwindow*)
{
  std::cout << "Window has been closed." << std::endl;
}


Window::Window()
  : Window("Application", 720, 480)
{

}

Window::Window(const std::string& title, int width, int height, bool fullscreen)
  : m_title{title}, m_width{width}, m_height{height}
{
  // Set the OpenGL version.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, constants::OPENGL_VERSION);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, constants::OPENGL_VERSION);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // Retrieve main monitor and its dimensions.
  int monitorXpos, monitorYpos, monitorWidth, monitorHeight;
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();;
  glfwGetMonitorWorkarea(monitor, &monitorXpos, &monitorYpos,
                         &monitorWidth, &monitorHeight);
  
  // Create a window.
  if (fullscreen)
  {
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), monitor, NULL);
  }
  else
  {
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
  }
  
  // Check it window was successfully created.
  if (!m_window)
  {
    std::cerr << "ERROR::main could not open window with GLFW3" << std::endl;
    return;
  }

  // Make the window's context current.
  glfwMakeContextCurrent(m_window);

  // Lock the window aspect ratio.
  glfwSetWindowAspectRatio(m_window, 16, 9);

  // Set the window location to the top left corner, account for the window
  // borders.
  int left, top, right, bottom;
  glfwGetWindowFrameSize(m_window, &left, &top, &right, &bottom);
  glfwSetWindowPos(m_window, monitorXpos, monitorYpos+top);

  // Set callback functions.
  glfwSetWindowSizeCallback(m_window, window_size_callback);
  glfwSetFramebufferSizeCallback(m_window, window_size_callback);
  glfwSetWindowPosCallback(m_window, window_pos_callback);
  glfwSetWindowCloseCallback 	(m_window, window_close_callback);

  // // Enable automatic window resizing.
  // glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
  
  // Disable cursor.
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
  close();
}

void Window::close()
{
  glfwSetWindowShouldClose(m_window, 1);
}

int Window::isAlive()
{
  return !glfwWindowShouldClose(m_window);
}

void Window::getSize(int& width, int& height) const
{
  glfwGetWindowSize(m_window, &width, &height);
}

void Window::setSize(const int width, const int height)
{
  m_width = width;
  m_height = height;
  glfwSetWindowSize(m_window, width, height);
}

void Window::setResolution(const WindowResolution resolution)
{
  int width, height;
  switch(resolution)
  {
  case WindowResolution::r480:
    width = 720;
    height = 480;
    break;
  case WindowResolution::r720:
    width = 1280;
    height = 720;
    break;
  case WindowResolution::r1080:
    width = 1920;
    height = 1080;
    break;
  default:
    std::cout << "ERROR::Window Unknown resolution " << std::endl;
    return;
  }

  setSize(width, height);
}

void Window::setFullscreen() const
{
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);  
  glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height,
                       mode->refreshRate);
}

void Window::setWindowed() const
{
  glfwSetWindowMonitor(m_window, NULL, m_xposition, m_yposition,
                       m_width, m_height, 0);
}

bool Window::isFullscreen() const
{
  if (glfwGetWindowMonitor(m_window) == NULL)
  {
    return false;
  }
  return true;
}

void Window::swapBuffers() const
{
  // Swap front and back buffers.
  glfwSwapBuffers(m_window);
}