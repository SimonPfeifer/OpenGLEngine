#pragma once

#include "Gui.h"
#include "Renderer.h"
#include "Scene.h"
#include "Window.h"

class Application
{
public:
  Application();
  ~Application();

  Application(const Application& other) = delete;
  Application& operator=(const Application& other) = delete;

  /**
   * @brief Main runtime loops of the application.
   */
  void run();
  
private:
  Window* m_window{};
  Gui* m_gui{};
  Scene* m_scene{};
  Renderer* m_renderer{};

  /** Timers. */
  float m_deltaTime = 0.0f;
  float m_lastTimeFrame = 0.0f;
};
