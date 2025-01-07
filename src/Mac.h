#pragma once

#include <memory>

#include "Grid.h"

template<class T>
struct Vec2
{
  Vec2(T x, T y) : x{x}, y{y} {};
  T x{};
  T y{};
};

class MAC
{
public:
  MAC(int width, int height);
  ~MAC() = default;

  void swap(MAC& other);

  int getWidth() const {return m_width;}
  int getHeight() const {return m_height;}

  Grid<float> pressure;
  Grid<float> velocityX;
  Grid<float> velocityY;

private:
  int m_width{};
  int m_height{};
};
