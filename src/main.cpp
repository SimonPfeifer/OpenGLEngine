// Disable inclusion of the environment header to fix header conflicts.
#define GLFW_INCLUDE_NONE

#include "Application.h"

#include <iostream>

int main()
{
  Application* app = new Application();
  app->run();
  delete app;

  return 0;
}
