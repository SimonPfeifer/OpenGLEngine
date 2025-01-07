#include <iostream>

#include "FluidSimulation.h"

FluidSimulation::FluidSimulation(float width, float height, int nCellsX,
                                 int nCellsY)
  : m_width{width}, m_height{height}, m_nCellsX{nCellsX}, m_nCellsY{nCellsY},
    m_mac(nCellsX, nCellsY), m_newMac(nCellsX, nCellsY),
    m_smoke(nCellsX, nCellsY), m_newSmoke(nCellsX, nCellsY),
    m_cellType(nCellsX, nCellsY),
    m_outputGrid(nCellsX, nCellsY)
{
  m_nCells = m_nCellsX * m_nCellsY;
  m_dx = m_width / static_cast<float>(m_nCellsX);
  m_dy = m_height / static_cast<float>(m_nCellsY);

  m_density = 1.0f;

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
        m_mac.velocityY.get(i,j) = -100.0f;
        m_mac.velocityY.get(i,j+1) = -100.0f;
      }
      if (i>=45 && i<55 && j==m_nCellsY-1)
      {
        m_mac.velocityY.get(i,j) = -100.0f;
        m_mac.velocityY.get(i,j+1) = -100.0f;
        m_smoke.get(i,j) = 1.0f;
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
  std::cout << "\n#### New step ####\n";
  std::cout << "deltaTime: " << deltaTime << "\n";
  printCellInfo(19,50);
  printCellInfo(80,50);

  // Add body forces to fluid velocity.
  std::cout << "Adding forces.\n";
  addForce(0.0f, -10.0f, deltaTime);

  // Calculate the pressure assuming incompressibility.
  std::cout << "Project." << std::endl;
  // calculatePressure2(deltaTime);
  project(deltaTime);

  std::cout << "Max velocity div.: " << maxVelocityDiv() << std::endl;

  // Advect velocity via velocity field.
  std::cout << "Advect velocity." << std::endl;
  advectVelocity(deltaTime);

  // Advect quantity via velocity field.
  std::cout << "Advect smoke." << std::endl;
  advectSmoke(deltaTime);
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
  float factor = m_dx * m_density / deltaTime;
  
  // For pressure logging.
  float minPressure = 100000.0f;
  float maxPressure = -100000.0f;
  float maxPressureDiff = -1.0f;

  // Save the velocity divergence as it is constant.
  Grid<float> dVelocity(m_nCellsX, m_nCellsY);

  int nSteps = 50;
  for (int n=0; n<nSteps; ++n)
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

        float sumPressure = 0.0f;
        float nFluidBoundaries = 0.0f;
        if (m_cellType.get(i-1,j)==CellType::FLUID)
        {
          sumPressure += m_mac.pressure.get(i-1,j);
          nFluidBoundaries++;
        }
        if (m_cellType.get(i+1,j)==CellType::FLUID)
        {
          sumPressure += m_mac.pressure.get(i+1,j);
          nFluidBoundaries++;
        }
        if (m_cellType.get(i,j-1)==CellType::FLUID)
        {
          sumPressure += m_mac.pressure.get(i,j-1);
          nFluidBoundaries++;
        }
        if (m_cellType.get(i,j+1)==CellType::FLUID)
        {
          sumPressure += m_mac.pressure.get(i,j+1);
          nFluidBoundaries++;
        }

        // Skip if we are surrounded by solid cells.
        if (nFluidBoundaries==0.0f)
          continue;

        if (n==0)
          dVelocity.get(i,j) = m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                               m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j);

        float relaxFactor = 1.0f;
        float newPressure = 1.0f/nFluidBoundaries * (sumPressure -
                            factor * dVelocity.get(i,j));
        newPressure = (1 - relaxFactor) * m_mac.pressure.get(i,j) +
                       relaxFactor * newPressure;
        minPressure = newPressure>minPressure ? minPressure : newPressure;
        maxPressure = newPressure<maxPressure ? maxPressure : newPressure;

        float pressureDiff = fabsf((newPressure - m_mac.pressure.get(i,j)) / m_mac.pressure.get(i,j));
        maxPressureDiff = pressureDiff<maxPressureDiff ? maxPressureDiff : pressureDiff;

        m_mac.pressure.get(i,j) = newPressure;
      }
    }
  }

  std::cout << "Min/max pressure: " << minPressure << " -  " << maxPressure << std::endl;
  std::cout << "Max pressure difference: " << maxPressureDiff << std::endl;
}

void FluidSimulation::calculatePressure2(float deltaTime)
{
  // Some dummy values.
  float pressureTerms = 0.01f * m_dx / deltaTime;
  float minPressure = 10000.0f;
  float maxPressure = -10000.0f;

  int nSteps = 50;
  for (int n=0; n<nSteps; ++n)
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
        float nFluidBoundaries = isFluidLeft + isFluidRight +
                                 isFluidDown + isFluidUp;
        if (nFluidBoundaries==0.0f)
          continue;

        float dVelocity = m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                          m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j);
        dVelocity /= nFluidBoundaries;

        // Over-relaxation
        dVelocity *= 1.0f;

        if (n==0)
          m_mac.pressure.get(i,j) = 0.0f;

        m_mac.pressure.get(i,j)    += pressureTerms * -dVelocity;
        m_mac.velocityX.get(i,j)   += dVelocity * isFluidLeft;
        m_mac.velocityX.get(i+1,j) -= dVelocity * isFluidRight;
        m_mac.velocityY.get(i,j)   += dVelocity * isFluidDown;
        m_mac.velocityY.get(i,j+1) -= dVelocity * isFluidUp;

        minPressure = m_mac.pressure.get(i,j)>minPressure ? minPressure : m_mac.pressure.get(i,j);
        maxPressure = m_mac.pressure.get(i,j)<maxPressure ? maxPressure : m_mac.pressure.get(i,j);
        
        // if (i==50 && j==50)
        // {
        //   std::cout << "Step: " << n << " - dVelocity: " << dVelocity << std::endl;
        //   std::cout << "Step: " << n << " - pressure: " << m_mac.pressure.get(i,j) << std::endl;
        // }
      }
    }
  }

  std::cout << "Min/max pressure: " << minPressure << " -  " << maxPressure << std::endl;
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
        // std::cout << "advectVelocity() - Non-fluid cell: " << i << ", " << j << std::endl;
        m_newMac.velocityX.get(i,j) = m_mac.velocityX.get(i,j);
        m_newMac.velocityX.get(i+1,j) = m_mac.velocityX.get(i+1,j);
        m_newMac.velocityY.get(i,j) = m_mac.velocityY.get(i,j);
        m_newMac.velocityY.get(i,j+1) = m_mac.velocityY.get(i,j+1);
        continue;
      }

      // Position of the centre of the cell.
      // std::cout << "advectVelocity() - Advect: " << i << ", " << j << std::endl;
      float xMid = static_cast<float>(i);
      float yMid = static_cast<float>(j);
      if (m_cellType.get(i-1,j)==CellType::FLUID)
      {
        // float x = xMid - 0.5f;
        // float y = yMid;
        // traceVelocity(x, y, deltaTime);
        float vx = m_mac.velocityX.get(i,j);
        float vy = m_mac.velocityY.sample(xMid-0.5f, yMid+0.5f);
        float x = xMid - deltaTime * vx;
        float y = yMid - deltaTime * vy;
        m_newMac.velocityX.get(i,j) = m_mac.velocityX.sample(x, y);
      }

      if (m_cellType.get(i,j-1)==CellType::FLUID)
      {
        // float x = xMid;
        // float y = yMid - 0.5f;
        // traceVelocity(x, y, deltaTime);
        float vx = m_mac.velocityX.sample(xMid+0.5f, yMid-0.5f);
        float vy = m_mac.velocityY.get(i,j);
        float x = xMid - deltaTime * vx;
        float y = yMid - deltaTime * vy;
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
        // std::cout << "advectSmoke() - Non-fluid cell: " << i << ", " << j << std::endl;
        m_newSmoke.get(i,j) = m_smoke.get(i,j);
        continue;
      }

      // Position of the centre of the cell.
      // std::cout << "advectSmoke() - Advect: " << i << ", " << j << std::endl;
      float x = static_cast<float>(i);
      float y = static_cast<float>(j);
      float vx = m_mac.velocityX.sample(x+0.5f, y);
      float vy = m_mac.velocityY.sample(x, y+0.5f);
      x -= deltaTime * vx;
      y -= deltaTime * vy;
      // traceVelocity(x, y, deltaTime);
      m_newSmoke.get(i,j) = m_smoke.sampleCubic(x, y);
    }
  }

  // std::cout << "advectSmoke() - Swap grid." << std::endl;;
  m_smoke.swap(m_newSmoke);
  // std::cout << "advectSmoke() - Done." << std::endl;
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

void FluidSimulation::printCellInfo(int i, int j) const
{ 
  std::string iStr = std::to_string(i);
  std::string jStr = std::to_string(j);
  std::cout << "Cell(" << iStr << "," << jStr << ")\n";
  std::cout << "Pressure: " << m_mac.pressure.get(i,j) << "\n";
  std::cout << "VelocityX: " << m_mac.velocityX.get(i,j) << ", " << m_mac.velocityX.get(i+1,j) << "\n";
  std::cout << "VelocityY: " << m_mac.velocityY.get(i,j) << ", " << m_mac.velocityY.get(i,j+1) << "\n";
  float velocityDiv = 1.0 / m_dx *
                      (m_mac.velocityX.get(i+1,j) - m_mac.velocityX.get(i,j) +
                       m_mac.velocityY.get(i,j+1) - m_mac.velocityY.get(i,j));
  std::cout << "Velocity div.: " << velocityDiv << std::endl;
  std::cout << "Smoke: " << m_smoke.get(i,j) << std::endl;
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