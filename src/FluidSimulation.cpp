#include <cmath>
#include <iostream>

#include "FluidSimulation.h"

FluidSimulation::FluidSimulation(unsigned int nCellsX, unsigned int nCellsY, float dx)
  : m_nCellsX(static_cast<int>(nCellsX)), m_nCellsY{static_cast<int>(nCellsY)},
    m_nCells{m_nCellsX*m_nCellsY}, m_dx{dx}, 
    m_width{static_cast<float>(nCellsX)*dx}, m_height{nCellsY*dx},
    m_forceX{0.0f}, m_forceY{-10.0f},
    m_density{1.0f}, m_relaxFactor{1.0f}, m_deltaTime{0.001f}, m_nSweeps{100},
    m_isPaused{false}, 
    m_mac(nCellsX, nCellsY), m_newMac(nCellsX, nCellsY),
    m_dVelocity(m_nCellsX, m_nCellsY),
    m_smoke(nCellsX, nCellsY), m_newSmoke(nCellsX, nCellsY),
    m_cellType(nCellsX, nCellsY),
    m_outputGrid(nCellsX, nCellsY),
    m_residuals(nCellsX, nCellsY)
{
  // Initialize each cell.
  for (int i=0; i<(m_nCellsX); ++i)
  {
    for (int j=0; j<(m_nCellsY); ++j)
    {
      // Not needed since Grid<T> default initialized but good to
      // be explicit.
      m_cellType.get(i,j) = CellType::FLUID;
      m_mac.pressure.get(i,j) = 100.0f * m_dx;
      m_smoke.get(i,j) = 0.0f;
    }
  }

  // Setup default boundary conditions.
  for (int i=0; i<(m_nCellsX); ++i)
  {
    for (int j=0; j<(m_nCellsY); ++j)
    {
      if (i==0 || i==m_nCellsX-1 || j==0 || j==m_nCellsY-1)
      {
        m_cellType.get(i,j) = CellType::SOLID;
        m_mac.pressure.get(i,j) = 0.0f;
        m_mac.velocityX.get(i,j) = 0.0f;
        m_mac.velocityX.get(i+1,j) = 0.0f;
        m_mac.velocityY.get(i,j) = 0.0f;
        m_mac.velocityY.get(i,j+1) = 0.0f;
        m_smoke.get(i,j) = 0.0f;
      }
    }
  }

  // Custom conditions
  for (int i=0; i<(m_nCellsX); ++i)
  {
    for (int j=0; j<(m_nCellsY); ++j)
    {
      
      // // Wind tunnel inflow.
      // if (i==0 && j>0 && j<m_nCellsY-1)
      // {
      //   m_mac.velocityX.get(i,j) = 100.0f;
      //   m_mac.velocityX.get(i+1,j) = 100.0f;
      // }
      // if (i==0 && j>=45 && j<55)
      // {
      //   m_smoke.get(i,j) = 1.0f;
      // }

      // // Wind tunnel outflow.
      // if (i==m_nCellsX-1)
      // {
      //   m_mac.velocityX.get(i,j) = 100.0f;
      //   m_mac.velocityX.get(i+1,j) = 100.0f;
      //   m_smoke.get(i,j) = 0.0f;
      // }

      // // Solid obstacle.
      // if (i>=10 && i<30 && j>=40 && j<60)
      // {
      //   m_cellType.get(i,j) = CellType::SOLID;
      //   m_mac.velocityX.get(i,j) = 0.0f;
      //   m_mac.velocityX.get(i+1,j) = 0.0f;
      //   m_mac.velocityY.get(i,j) = 0.0f;
      //   m_mac.velocityY.get(i,j+1) = 0.0f;
      //   m_smoke.get(i,j) = 0.0f;
      // }

      // Top pour.
      if (i>0 && i<m_nCellsY-1 && j==m_nCellsY-1)
      {
        m_mac.velocityY.get(i,j) = -100.0f * m_dx;
        m_mac.velocityY.get(i,j+1) = -100.0f * m_dx;
      }
      if (i>=45 && i<55 && j==m_nCellsY-1)
      {
        m_mac.velocityY.get(i,j) = -100.0f * m_dx;
        m_mac.velocityY.get(i,j+1) = -100.0f * m_dx;
        m_smoke.get(i,j) = 1.0f;
      }

      // Bottom outlet.
      if (i>0 && i<m_nCellsY-1 && j==0)
      {
        m_mac.velocityY.get(i,j) = -100.0f * m_dx;
        m_mac.velocityY.get(i,j+1) = -100.0f * m_dx;
      }

      // Solid obstacle.
      if (i>=40 && i<60 && j>=(m_nCellsY-1)-30 && j<(m_nCellsY-1)-10)
      {
        m_cellType.get(i,j) = CellType::SOLID;
        m_mac.velocityX.get(i,j) = 0.0f;
        m_mac.velocityX.get(i+1,j) = 0.0f;
        m_mac.velocityY.get(i,j) = 0.0f;
        m_mac.velocityY.get(i,j+1) = 0.0f;
        m_smoke.get(i,j) = 0.0f;
      }

      // // Smoke shock.
      // if (i>0 && i<30 && j>45 && j<55)
      // {
      //   m_mac.velocityX.get(i,j) = 100.0f;
      //   m_smoke.get(i,j) = 1.0f;
      // }
      
    }
  }

}

void FluidSimulation::update(float deltaTime)
{
  deltaTime = deltaTime + 1.0f;
  if (!m_isPaused)
  {
    // Add body forces to fluid velocity.
    addForce(m_forceX, m_forceY, m_deltaTime);

    // Calculate the pressure assuming incompressibility.
    project(m_deltaTime);

    // Advect velocity via velocity field.
    advectVelocity(m_deltaTime);

    // Advect quantity via velocity field.
    advectSmoke(m_deltaTime);

    // Calculate the residuals of to measure convergance.
    calculateResidual(m_deltaTime);
  }
}

void FluidSimulation::addForce(float forceX, float forceY, float deltaTime)
{
  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      if (m_cellType.get(i,j)==CellType::FLUID)
      {
        // Check neighbor in X direction.
        if (m_cellType.get(i-1,j)==CellType::FLUID)
          m_mac.velocityX.get(i,j) += forceX * deltaTime;

        // Check neighbor in Y direction.
        if (m_cellType.get(i,j-1)==CellType::FLUID)
          m_mac.velocityY.get(i,j) += forceY * deltaTime;
      }
    }
  }
}

void FluidSimulation::calculatePressure(float deltaTime)
{
  // Constants.
  float velocityTerm = m_dx * m_density / deltaTime;
  
  // For pressure logging.
  m_minPressure = 1e8;
  m_maxPressure = -1e8;
  m_maxPressureDiff = -1.0f;

  for (int n=0; n<m_nSweeps; ++n)
  {
    for (int ii=0; ii<m_nCellsX; ++ii)
    {
      for (int jj=0; jj<m_nCellsY; ++jj)
      {
        // Reverse sweep Gauss-Seidel every other loop.
        int i = n%2 ? (m_nCellsX-1) - ii : ii;
        int j = n%2 ? (m_nCellsY-1) - jj : jj;

        if (m_cellType.get(i,j)!=CellType::FLUID)
          continue;

        // Check which neighbors are fluid cells.
        float isFluidLeft = 0.0f;
        float isFluidRight = 0.0f;
        float isFluidDown = 0.0f;
        float isFluidUp = 0.0f;
        if (m_cellType.get(i-1,j)==CellType::FLUID)
          isFluidLeft = 1.0f;
        if (m_cellType.get(i+1,j)==CellType::FLUID)
          isFluidRight = 1.0f;
        if (m_cellType.get(i,j-1)==CellType::FLUID)
          isFluidDown = 1.0f;
        if (m_cellType.get(i,j+1)==CellType::FLUID)
          isFluidUp = 1.0f;
          
        // Skip if we are surrounded by solid cells.
        float nFluidBoundaries = isFluidLeft + isFluidRight +
                                 isFluidDown + isFluidUp;
        if (nFluidBoundaries==0.0f)
          continue;

        // // Alternative method, forcing velocity divergence to be zero.
        // dVelocity.get(i,j) = m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
        //                      m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j);
        // dVelocity.get(i,j) /= nFluidBoundaries;

        // // Over-relaxation
        // dVelocity.get(i,j) *= 1.0f;

        // if (n==0)
        //   m_mac.pressure.get(i,j) = 0.0f;
        
        // float newPressure = m_mac.pressure.get(i,j) - velocityTerm * dVelocity.get(i,j);
        // m_mac.velocityX.get(i,j)   += dVelocity.get(i,j) * isFluidLeft;
        // m_mac.velocityX.get(i+1,j) -= dVelocity.get(i,j) * isFluidRight;
        // m_mac.velocityY.get(i,j)   += dVelocity.get(i,j) * isFluidDown;
        // m_mac.velocityY.get(i,j+1) -= dVelocity.get(i,j) * isFluidUp;

        // Only need to calculate this once since velocity is not updated.
        if (n==0)
          m_dVelocity.get(i,j) = m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                                 m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j);

        // Calculate the new pressure.
        float sumPressure = isFluidLeft * m_mac.pressure.get(i-1,j) +
                            isFluidRight * m_mac.pressure.get(i+1,j) +
                            isFluidDown * m_mac.pressure.get(i,j-1) +
                            isFluidUp * m_mac.pressure.get(i,j+1);
        float newPressure = 1.0f/nFluidBoundaries * (sumPressure -
                            velocityTerm * m_dVelocity.get(i,j));
        
        // Over-relaxation.
        newPressure = (1 - m_relaxFactor) * m_mac.pressure.get(i,j) +
                       m_relaxFactor * newPressure;

        // Logging.
        m_minPressure = newPressure<m_minPressure ? newPressure : m_minPressure;
        m_maxPressure = newPressure>m_maxPressure ? newPressure : m_maxPressure;
        float pressureDiff = fabsf((newPressure - m_mac.pressure.get(i,j)) / m_mac.pressure.get(i,j));
        m_maxPressureDiff = pressureDiff<m_maxPressureDiff ? m_maxPressureDiff : pressureDiff;

        // Update with the new pressure value.
        m_mac.pressure.get(i,j) = newPressure;
      }
    }
  }
}

void FluidSimulation::calculateVelocity(float deltaTime)
{
  // Constants.
  float factor = deltaTime / m_density / m_dx;

  // Skip over outer most boundaries.
  for (int i=1; i<m_nCellsX; ++i)
  {
    for (int j=1; j<m_nCellsY; ++j)
    {
      if (m_cellType.get(i,j)!=CellType::FLUID)
        continue;

      if (m_cellType.get(i-1,j)==CellType::FLUID)
        m_mac.velocityX.get(i,j) -= factor * (m_mac.pressure.get(i,j) -
                                              m_mac.pressure.get(i-1,j));

      if (m_cellType.get(i,j-1)==CellType::FLUID)
        m_mac.velocityY.get(i,j) -= factor * (m_mac.pressure.get(i,j) -
                                              m_mac.pressure.get(i,j-1));
    }
  }
}

void FluidSimulation::project(float deltaTime)
{
  calculatePressure(deltaTime);
  calculateVelocity(deltaTime);
}

void FluidSimulation::advectVelocity(float deltaTime)
{
  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      // Simply copy non-fluid velocities.
      if (m_cellType.get(i,j)!=CellType::FLUID)
      {
        m_newMac.velocityX.get(i,j) = m_mac.velocityX.get(i,j);
        m_newMac.velocityX.get(i+1,j) = m_mac.velocityX.get(i+1,j);
        m_newMac.velocityY.get(i,j) = m_mac.velocityY.get(i,j);
        m_newMac.velocityY.get(i,j+1) = m_mac.velocityY.get(i,j+1);
        continue;
      }

      // Position of the centre of the cell.
      float xMid = static_cast<float>(i);
      float yMid = static_cast<float>(j);
      if (m_cellType.get(i-1,j)==CellType::FLUID)
      {
        // float x = xMid - 0.5f;
        // float y = yMid;
        // traceVelocity(x, y, deltaTime);
        float vx = m_mac.velocityX.get(i,j);
        float vy = m_mac.velocityY.sample(xMid-0.5f, yMid+0.5f);
        float x = xMid - deltaTime * vx / m_dx;
        float y = yMid - deltaTime * vy / m_dx;
        m_newMac.velocityX.get(i,j) = m_mac.velocityX.sample(x, y);
      }

      if (m_cellType.get(i,j-1)==CellType::FLUID)
      {
        // float x = xMid;
        // float y = yMid - 0.5f;
        // traceVelocity(x, y, deltaTime);
        float vx = m_mac.velocityX.sample(xMid+0.5f, yMid-0.5f);
        float vy = m_mac.velocityY.get(i,j);
        float x = xMid - deltaTime * vx / m_dx;
        float y = yMid - deltaTime * vy / m_dx;
        m_newMac.velocityY.get(i,j) = m_mac.velocityY.sample(x, y);
      }
    }
  }

  m_mac.velocityX.swap(m_newMac.velocityX);
  m_mac.velocityY.swap(m_newMac.velocityY);
}

void FluidSimulation::advectSmoke(float deltaTime)
{
  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      // Simply copy non-fluid velocities.
      if (m_cellType.get(i,j)!=CellType::FLUID)
      {
        m_newSmoke.get(i,j) = m_smoke.get(i,j);
        continue;
      }

      // Position of the centre of the cell.
      float x = static_cast<float>(i);
      float y = static_cast<float>(j);
      float vx = m_mac.velocityX.sample(x+0.5f, y);
      float vy = m_mac.velocityY.sample(x, y+0.5f);
      x -= deltaTime * vx / m_dx;
      y -= deltaTime * vy / m_dx;
      // traceVelocity(x, y, deltaTime);
      m_newSmoke.get(i,j) = m_smoke.sampleCubic(x, y);
    }
  }

  m_smoke.swap(m_newSmoke);
}

void FluidSimulation::traceVelocity(float& x, float& y, float deltaTime)
{
  // Third order Runge-Kutta
  // k1 = f(qn)
  // k2 = f(qn + 1/2*dt*k1)
  // k3 = f(qn + 3/4*dt*k2)
  // qn+1 = qn + 2/9*dt*k1 + 3/9*dt*k2 + 4/9*dt*k3

  // float k1x = -m_mac.velocityX.sample(x, y);
  // float k1y = -m_mac.velocityY.sample(x, y);

  // float fac1 = 0.5f * deltaTime;
  // float x1 = x+fac1*k1x;
  // float y1 = y+fac1*k1y;
  // float k2x = 0.0f;
  // float k2y = 0.0f;
  // // Only keep tracing velocity if we are still in a fluid cell.
  // // if (m_cellType.get(static_cast<int>(x1+0.5f),static_cast<int>(y1+0.5f))==CellType::FLUID)
  // // {
  // // }
  //   k2x = -m_mac.velocityX.sample(x1, y1);
  //   k2y = -m_mac.velocityY.sample(x1, y1);

  // float fac2 = 0.75f * deltaTime;
  // float x2 = x+fac2*k2x;
  // float y2 = y+fac2*k2y;
  // float k3x = 0.0f;
  // float k3y = 0.0f;
  // // if (m_cellType.get(static_cast<int>(x2+0.5f),static_cast<int>(y2+0.5f))==CellType::FLUID)
  // // {
  // // }
  //   k3x = -m_mac.velocityX.sample(x2, y2);
  //   k3y = -m_mac.velocityY.sample(x2, y2);
  
  // fac1 = 2.0f/9.0f*deltaTime;
  // fac2 = 3.0f/9.0f*deltaTime;
  // float fac3 = 4.0f/9.0f*deltaTime;
  // x += fac1*k1x + fac2*k2x + fac3*k3x;
  // y += fac1*k1y + fac2*k2y + fac3*k3y;
  x -= deltaTime * m_mac.velocityX.sample(x, y);
  y -= deltaTime * m_mac.velocityY.sample(x, y);
}

float* FluidSimulation::outputPressure()
{
  // Return a copy of the pressure grid.
  return m_mac.pressure.data();
}

float* FluidSimulation::outputVelocityX()
{
  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      m_outputGrid.get(i,j) = m_mac.velocityX.sample(i+0.5f, j);
    }
  }
  return m_outputGrid.data();
}

float* FluidSimulation::outputVelocityY()
{
  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      m_outputGrid.get(i,j) = m_mac.velocityY.sample(i, j+0.5f);
    }
  }
  return m_outputGrid.data();
}

float* FluidSimulation::outputSmoke()
{
  // Return a copy of the pressure grid.
  return m_smoke.data();
}

void FluidSimulation::getCellInfo(int i, int j, CellInfo& cellInfo) const
{ 
  cellInfo.i = i;
  cellInfo.j = j;
  cellInfo.pressure = m_mac.pressure.get(i,j);
  cellInfo.velocityX = m_mac.velocityX.get(i,j);
  cellInfo.velocityY = m_mac.velocityY.get(i,j);
  cellInfo.velocityDiv = 1.0 / m_dx *
                        (m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                         m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j));
  cellInfo.smoke = m_smoke.get(i,j);
  cellInfo.residual = m_residuals.get(i,j);
}

void FluidSimulation::calculateResidual(float deltaTime)
{
  // Constants.
  float factor = deltaTime / m_density / m_dx / m_dx;

  float count = 0.0f;
  m_residualL1 = 0.0f;
  m_residualL2 = 0.0f;
  m_residualLInf = 0.0f;
  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      // Simply copy non-fluid velocities.
      if (m_cellType.get(i,j)!=CellType::FLUID)
        continue;

      m_dVelocity.get(i,j) = m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                             m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j);

      // Check which neighbors are fluid cells.
      float isFluidLeft = 0.0f;
      float isFluidRight = 0.0f;
      float isFluidDown = 0.0f;
      float isFluidUp = 0.0f;
      if (m_cellType.get(i-1,j)==CellType::FLUID)
        isFluidLeft = 1.0f;
      if (m_cellType.get(i+1,j)==CellType::FLUID)
        isFluidRight = 1.0f;
      if (m_cellType.get(i,j-1)==CellType::FLUID)
        isFluidDown = 1.0f;
      if (m_cellType.get(i,j+1)==CellType::FLUID)
        isFluidUp = 1.0f;
      
      // Skip if we are surrounded by solid cells.
      float nFluidBoundaries = isFluidLeft + isFluidRight +
                               isFluidDown + isFluidUp;
      if (nFluidBoundaries==0.0f)
        continue;                              

      // Calculate the new pressure.
      float sumPressure = nFluidBoundaries * m_mac.pressure.get(i,j) -
                          isFluidLeft * m_mac.pressure.get(i-1,j) -
                          isFluidRight * m_mac.pressure.get(i+1,j) -
                          isFluidDown * m_mac.pressure.get(i,j-1) -
                          isFluidUp * m_mac.pressure.get(i,j+1);
      m_residuals.get(i,j) = std::fabs(factor * sumPressure +
                                       m_dVelocity.get(i,j) / m_dx);
      
      m_residualL1 += m_residuals.get(i,j);
      m_residualL2 += m_residuals.get(i,j) * m_residuals.get(i,j);
      m_residualLInf = m_residuals.get(i,j)>m_residualLInf ? m_residuals.get(i,j) : m_residualLInf;
      count++;
    }
  }

  m_residualL1 /= count;
  m_residualL2 = std::sqrt(m_residualL2 / count);
}

float FluidSimulation::maxVelocityDiv() const
{
  float scale = 1.0f / m_dx;
  float maxVelocityDiv = -1.0f;

  for (int i=0; i<m_nCellsX; ++i)
  {
    for (int j=0; j<m_nCellsY; ++j)
    {
      if (m_cellType.get(i,j)!=CellType::FLUID)
        continue;

      float velocityDiv = scale *
                          (m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                           m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j));
      velocityDiv = fabsf(velocityDiv);
      maxVelocityDiv = velocityDiv>maxVelocityDiv ? velocityDiv : maxVelocityDiv;
    }
  }

  return maxVelocityDiv;
}