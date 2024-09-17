#pragma once

#include "Camera.h"

#include <glm/glm.hpp>

class Shadow
{
public:
  Shadow(int mapWidht, int mapHeight, int nCascadeSlices);
  ~Shadow();

  glm::mat4 *view;
  glm::mat4 *projection;

  void shadowMatrices(float lightHeight, glm::vec3 &lightDirection,
                      Camera &camera);

private:
  int nShadowCascadeSlices;
  int shadowMapWidth;
  int shadowMapHeight;

  void frustumSliceZDistances(Camera &camera, float distances[]);
  void frustumSliceCorners(Camera &camera, float near, float far,
                                   glm::vec4 (&cornersOut)[8]);
  void frustumSliceBoundsSphere(const glm::vec3 &center,
                                const glm::vec4 (&frustumCorners)[8],
                                glm::vec3 &minBounds,
                                glm::vec3 &maxBounds);
  void projectionTexelOffset(const glm::mat4 &viewMatrix,
                             glm::mat4 &projectionMatrix);
};