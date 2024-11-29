#pragma once

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"

/**
 * @brief A wrapper and manager of the ImGUI library.
 */
class Gui
{
public:
  Gui(const Window& window);
  ~Gui();

  /**
   * @brief Render all of the specified ImGUI windows.
   */
  void render();

private:

};