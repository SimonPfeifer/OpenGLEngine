#pragma once

/**
 * @brief An interface for a GUI window.
 */
class GuiWindow
{
public:
  GuiWindow() = default;
  ~GuiWindow() = default;
  
  virtual void update() = 0;

private:
};
