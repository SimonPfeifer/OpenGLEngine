#include <algorithm>
#include <cmath>
#include <memory>

#include "Grid.h"
#include "Mac.h"

MAC::MAC(int width, int height)
  : pressure(width, height), velocityX(width+1, height), velocityY(width, height+1),
    m_width{width}, m_height{height}
{

}

void MAC::swap(MAC& other)
{
  using std::swap;
  swap(m_width, other.m_width);
  swap(m_height, other.m_height);
  pressure.swap(other.pressure);
  velocityX.swap(other.velocityX);
  velocityY.swap(other.velocityY);
}
