#include "FluidSimulation.h"
#include "GuiWindowFluidSimulation.h"

#include "imgui.h"
#include <iostream>

GuiWindowFluidSimulation::GuiWindowFluidSimulation(FluidSimulation& fluidSimulation)
  : m_fluidSimulation{fluidSimulation},
    m_nCellsX{fluidSimulation.getNCellsX()}, m_nCellsY{fluidSimulation.getNCellsY()},
    m_width{fluidSimulation.getWidth()}, m_height{fluidSimulation.getHeight()},
    m_dx{fluidSimulation.getDx()},
    m_forceX{fluidSimulation.getForceX()}, m_forceY{fluidSimulation.getForceY()},
    m_useFrameTime{true}, m_deltaTime{0.01f}, m_isPaused{false},
    m_nSweeps{100}, m_relaxFactor{fluidSimulation.getRelaxFactor()}
{

}

void GuiWindowFluidSimulation::update()
{
  ImGui::Begin("Fluid Simulation");

  ImGuiIO& io = ImGui::GetIO();
  ImGui::Text("Frame time: %.4f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

  ImGui::SeparatorText("Global properties");
  ImGui::Text("Ncells X:  %d", m_nCellsX);
  ImGui::Text("Ncells Y:  %d", m_nCellsY);
  ImGui::Text("Width (m):  %.2f", m_width);
  ImGui::Text("Height (m):  %.2f", m_height);
  ImGui::Text("Cell size (m):  %.4f", m_dx);
  ImGui::SliderFloat("Force in X (m/s2): ", &m_forceX, -100.0f, 100.0f);
  m_fluidSimulation.setForceX(m_forceX);
  ImGui::SliderFloat("Force in Y (m/s2): ", &m_forceY, -100.0f, 100.0f);
  m_fluidSimulation.setForceY(m_forceY);
  ImGui::Checkbox("Use frame time", &m_useFrameTime);
  ImGui::SliderFloat("Time step (s)", &m_deltaTime, 0.001f, 0.05f);
  if (m_useFrameTime)
    m_fluidSimulation.setDeltaTime(1.0f/io.Framerate);
  else
    m_fluidSimulation.setDeltaTime(m_deltaTime);
  ImGui::Checkbox("Pause", &m_isPaused);
  m_fluidSimulation.setIsPaused(m_isPaused);

  ImGui::SeparatorText("Convergence");
  ImGui::SliderInt("Num. sweeps: ", &m_nSweeps, 1, 1000);
  m_fluidSimulation.setNSweeps(m_nSweeps);
  ImGui::SliderFloat("Relaxation factor", &m_relaxFactor, 0.0f, 2.0f);
  m_fluidSimulation.setRelaxFactor(m_relaxFactor);
  ImGui::Text("r1:  %.10f", m_fluidSimulation.getResidualL1());
  ImGui::Text("r2:  %.10f", m_fluidSimulation.getResidualL2());
  ImGui::Text("rInf:  %.10f", m_fluidSimulation.getResidualLInf());
  ImGui::Text("Min. pressure:  %.4f", m_fluidSimulation.getMinPressure());
  ImGui::Text("Max. pressure:  %.4f", m_fluidSimulation.getMaxPressure());
  ImGui::Text("Max. pressure difference:  %.4f", m_fluidSimulation.getMaxPressureDiff());
  
  ImGui::SeparatorText("Cell properties");
  ImGui::SliderInt("i: ", &m_cellInfo.i, 0, m_fluidSimulation.getNCellsX()-1);
  ImGui::SliderInt("j: ", &m_cellInfo.j, 0, m_fluidSimulation.getNCellsY()-1);
  m_fluidSimulation.getCellInfo(m_cellInfo.i, m_cellInfo.j, m_cellInfo);
  ImGui::Text("Pressure:  %.4f", m_cellInfo.pressure);
  ImGui::Text("VelocityX:  %.4f", m_cellInfo.velocityX);
  ImGui::Text("VelocityY:  %.4f", m_cellInfo.velocityY);
  ImGui::Text("Velocity divergence:  %.4f", m_cellInfo.velocityDiv);
  ImGui::Text("Smoke:  %.4f", m_cellInfo.smoke);
  ImGui::Text("Residual:  %.4f", m_cellInfo.residual);

  ImGui::SeparatorText("Rendering");
  ImGui::ColorEdit3("Render color", (float*)&m_color);

  ImGui::SeparatorText("Misc");
  ImGui::Checkbox("Demo window", &m_show_demo_window);
  if (m_show_demo_window)
    ImGui::ShowDemoWindow(&m_show_demo_window);

  ImGui::End();
}