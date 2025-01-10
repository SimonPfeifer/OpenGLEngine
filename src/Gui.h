#pragma once

#include <memory>
#include <vector>

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GuiWindow.h"
#include "Window.h"

/**
 * @brief A wrapper and manager of the ImGUI library.
 */
class Gui
{
public:
  Gui(const Window& window);
  ~Gui();

  void addWindow(std::unique_ptr<GuiWindow> window);

  /**
   * @brief Render all of the specified ImGUI windows.
   */
  void render();

private:
  std::vector<std::unique_ptr<GuiWindow>> m_guiWindows;

};