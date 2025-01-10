#include "GuiWindow.h"
#include "FluidSimulation.h"

#include <glm/glm.hpp>

class GuiWindowFluidSimulation : public GuiWindow
{
public:
  GuiWindowFluidSimulation(FluidSimulation& fluidSimulation);
  ~GuiWindowFluidSimulation() = default;

  void update();

private:
  FluidSimulation& m_fluidSimulation;

  // Global properties.
  int m_nCellsX{};
  int m_nCellsY{};
  float m_width{};
  float m_height{};
  float m_dx{};
  float m_forceX{};
  float m_forceY{};
  bool m_useFrameTime{};
  float m_deltaTime{};
  bool m_isPaused{};

  // Convergence.
  int m_nSweeps{};
  float m_relaxFactor{};

  // Cell properties.
  CellInfo m_cellInfo{};

  // Rendering.
  glm::vec3 m_color{};

  bool m_show_demo_window = false;
};