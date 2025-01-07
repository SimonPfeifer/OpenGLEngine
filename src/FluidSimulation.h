#pragma once

#include <vector>

#include "Grid.h"
#include "Mac.h"


enum class CellType
{
  FLUID = 0,
  SOLID = 1,
  FREE = 2
};

class FluidSimulation
{
public:
  FluidSimulation(float width, float height, int nCellsX, int nCellsY);
  ~FluidSimulation() = default;

  void update(float deltaTime);

  float* outputPressure();
  float* outputVelocityX();
  float* outputVelocityY();
  float* outputSmoke();

private:
  float m_width;
  float m_height;
  int m_nCellsX;
  int m_nCellsY;
  int m_nCells;
  float m_dx;
  float m_dy;

  float m_density;

  MAC m_mac;
  MAC m_newMac;
  Grid<float> m_smoke;
  Grid<float> m_newSmoke;
  Grid<CellType> m_cellType;
  Grid<float> m_outputGrid;

  void addForce(float forceX, float forceY, float deltaTime);
  void project(float deltaTime);
  void advectVelocity(float deltaTime);
  void advectSmoke(float deltaTime);

  void calculatePressure(float deltaTime);
  void calculatePressure2(float deltaTime);
  void calculateVelocity(float deltaTime);
  void traceVelocity(float& x, float& y, float deltaTime);

  void printCellInfo(int i, int j) const;
  float maxVelocityDiv() const;
};
