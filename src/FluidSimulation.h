#pragma once

#include <vector>

#include "Grid.h"
#include "Mac.h"

struct CellInfo
{
  int i{};
  int j{};
  float pressure{};
  float velocityX{};
  float velocityY{};
  float velocityDiv{};
  float smoke{};
  float residual{};
};

enum class CellType
{
  FLUID = 0,
  SOLID = 1,
  FREE = 2
};

class FluidSimulation
{
public:
  FluidSimulation(unsigned int nCellsX, unsigned int nCellsY, float dx);
  ~FluidSimulation() = default;

  void update(float deltaTime);

  float* outputPressure();
  float* outputVelocityX();
  float* outputVelocityY();
  float* outputSmoke();

  // Getters and setters.
  int getNCellsX() const {return m_nCellsX;}
  int getNCellsY() const {return m_nCellsY;}
  int getNCells() const {return m_nCells;}
  float getDx() const {return m_dx;}
  float getWidth() const {return m_width;}
  float getHeight() const {return m_height;}
  
  void setForceX(float forceX) {m_forceX = forceX;}
  float getForceX() {return m_forceX;}
  void setForceY(float forceY) {m_forceY = forceY;}
  float getForceY() {return m_forceY;}
  
  float getDensity() const {return m_density;}
  void setDensity(float density) {m_density = density;}
  float getRelaxFactor() const {return m_relaxFactor;}
  void setRelaxFactor(float relaxFactor) {m_relaxFactor = relaxFactor;}
  void setDeltaTime(float deltaTime) {m_deltaTime = deltaTime;}
  int getNSweeps() const {return m_nSweeps;}
  void setNSweeps(int nSweeps) {m_nSweeps = nSweeps;}
  
  float getMinPressure() {return m_minPressure;}
  float getMaxPressure() {return m_maxPressure;}
  float getMaxPressureDiff() {return m_maxPressureDiff;}
  
  bool getIsPaused() const {return m_isPaused;}
  void setIsPaused(bool isPaused) {m_isPaused = isPaused;}

  float getResidualL1() const {return m_residualL1;}
  float getResidualL2() const {return m_residualL2;}
  float getResidualLInf() const {return m_residualLInf;}

  void getCellInfo(int i, int j, CellInfo& cellInfo) const;

private:
  int m_nCellsX{};
  int m_nCellsY{};
  int m_nCells{};
  float m_dx{};
  float m_width{};
  float m_height{};

  float m_forceX{};
  float m_forceY{};

  float m_density{};
  float m_relaxFactor{};
  float m_deltaTime{};
  int m_nSweeps{};

  bool m_isPaused{};

  MAC m_mac;
  MAC m_newMac;
  Grid<float> m_dVelocity;
  Grid<float> m_smoke;
  Grid<float> m_newSmoke;
  Grid<CellType> m_cellType;
  Grid<float> m_outputGrid;

  float m_residualL1{};
  float m_residualL2{};
  float m_residualLInf{};
  Grid<float> m_residuals;
  
  float m_minPressure{};
  float m_maxPressure{};
  float m_maxPressureDiff{};

  void addForce(float forceX, float forceY, float deltaTime);
  void project(float deltaTime);
  void advectVelocity(float deltaTime);
  void advectSmoke(float deltaTime);

  void calculatePressure(float deltaTime);
  void calculateVelocity(float deltaTime);
  void traceVelocity(float& x, float& y, float deltaTime);

  void calculateResidual(float deltaTime);
  float maxVelocityDiv() const;
};
