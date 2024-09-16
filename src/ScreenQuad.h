#pragma once

#include "Mesh.h"

class ScreenQuad : public Mesh
{
public:
  ScreenQuad();
  ScreenQuad(float xmin, float xmax, float ymin, float ymax);

  void generate(float xmin, float xmax, float ymin, float ymax);

private:
  float xmin;
  float xmax;
  float ymin;
  float ymax;
};