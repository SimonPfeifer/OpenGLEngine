#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

template<class T>
class Grid
{
public:
  Grid(int width, int height)
    : m_width{width}, m_height{height}
  {
    m_size = width * height;
    m_data = std::make_unique<std::vector<T>>(m_size);
  }

  ~Grid() = default;

  Grid(const Grid<T>& other) = delete;
  Grid<T>& operator=(Grid<T> other) = delete;

  int getWidth() const {return m_width;}
  int getHeight() const {return m_height;}
  int getSize() const {return m_size;}

  #ifdef NDEBUG
  T& operator[](int idx) const {return (*m_data)[idx];}
  #else
  T& operator[](int idx) const {return (*m_data).at(idx);}
  #endif

  T& get(int i, int j) const
  {
    int idx = i + (j * m_width);
    
    #ifdef NDEBUG
    return (*m_data)[idx];
    #else
    return (*m_data).at(idx);
    #endif
  }

  T* data()
  {
    return m_data->data();
  }

  void swap(Grid<T>& other)
  {
    using std::swap;
    swap(m_width, other.m_width);
    swap(m_height, other.m_height);
    swap(m_size, other.m_size);
    swap(m_data, other.m_data);
  };

  T sample(float x, float y)
  {
    // Clamp the indices to the minimum and maximum.
    int i = std::clamp(static_cast<int>(x), 0, m_width-1);
    int j = std::clamp(static_cast<int>(y), 0, m_height-1);
    int iPlus1 = std::min(i+1, m_width-1);
    int jPlus1 = std::min(j+1, m_height-1);

    // Sample the grid with bilinear interpolation.
    float fracX = x - static_cast<int>(x);
    float fracY = y - static_cast<int>(y);
    T p12 = std::lerp(get(i,j), get(iPlus1,j), fracX);
    T p34 = std::lerp(get(i,jPlus1), get(iPlus1,jPlus1), fracX);
    return std::lerp(p12, p34, fracY);
  }

  T sampleCubic(float x, float y)
  {
     // w0 = −1/3 * s + 1/2 * s2 − 1/6 * s3 
     // w1 = 1 − s2 + 1/2 * (s3 − s)
     // w2 = s + 1/2 * (s2 − s3)
     // w3 = 1/6 * (s3 − s)

    // Clamp the indices to the minimum and maximum.
    int i = std::clamp(static_cast<int>(x), 0, m_width-1);
    int j = std::clamp(static_cast<int>(y), 0, m_height-1);
    int iMinus1 = std::max(i-1, 0);
    int jMinus1 = std::max(j-1, 0);
    int iPlus1 = std::min(i+1, m_width-1);
    int jPlus1 = std::min(j+1, m_height-1);
    int iPlus2 = std::min(i+2, m_width-1);
    int jPlus2 = std::min(j+2, m_height-1);

    // Sample the grid with cubic interpolation.
    // First in the X direction.
    float s = x - static_cast<int>(x);
    float s2 = s * s;
    float s3 = s2 * s;

    float w0 = -1.0f/3.0f * s + 0.5f * s2 - 1.0f/6.0f * s3;
    float w1 = 1.0f - s2 + 0.5f * (s3 - s);
    float w2 = s + 0.5f * (s2 - s3);
    float w3 = 1.0f/6.0f * (s3 - s);

    float q0 = w0*get(iMinus1,jMinus1) + w1*get(i,jMinus1) +
               w2*get(iPlus1,jMinus1) + w3*get(iPlus2,jMinus1);
    float q1 = w0*get(iMinus1,j) + w1*get(i,j) +
               w2*get(iPlus1,j) + w3*get(iPlus2,j);
    float q2 = w0*get(iMinus1,jPlus1) + w1*get(i,jPlus1) +
               w2*get(iPlus1,jPlus1) + w3*get(iPlus2,jPlus1);
    float q3 = w0*get(iMinus1,jPlus2) + w1*get(i,jPlus2) +
               w2*get(iPlus1,jPlus2) + w3*get(iPlus2,jPlus2);

    float t = y - static_cast<int>(y);
    float t2 = t * t;
    float t3 = t2 * t;

    w0 = -1.0f/3.0f * t + 0.5f * t2 - 1.0f/6.0f * t3;
    w1 = 1.0f - t2 + 0.5f * (t3 - t);
    w2 = t + 0.5f * (t2 - t3);
    w3 = 1.0f/6.0f * (t3 - t);

    return w0*q0 + w1*q1 + w2*q2 + w3*q3;
  }

private:
    int m_width{};
    int m_height{};
    int m_size{};

    std::unique_ptr<std::vector<T>> m_data;

};
