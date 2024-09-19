#pragma once

#include "Camera.h"

#include <glm/glm.hpp>

#include <vector>

class Shadow
{
public:
  Shadow(int mapWidht, int mapHeight, int nCascadeSlices, float weightLogLinear);

  void shadowMatrices(float lightHeight, glm::vec3 &lightDirection,
                      Camera &camera);

  void setNnCascadeSlices(int n) {nCascadeSlices = n; reset();}
  int getNCascadeSlices() const {return nCascadeSlices;}

  const std::vector<glm::mat4>& getView() const {return view;}
  const glm::mat4& getView(int index) const {return view[index];}

  const std::vector<glm::mat4>& getProjection() const {return projection;}
  const glm::mat4& getProjection(int index) const {return projection[index];}

  const std::vector<glm::mat4>& getViewProjection() const {return viewProjection;}
  const glm::mat4& getViewProjection(int index) const {return viewProjection[index];}

  const std::vector<float>& getSlicePlaneDistances() const {return slicePlaneDistances;}
  float getSlicePlaneDistances(int index) const {return slicePlaneDistances[index];}

private:
  int nCascadeSlices;
  int mapWidth;
  int mapHeight;

  std::vector<glm::mat4> view;
  std::vector<glm::mat4> projection;
  std::vector<glm::mat4> viewProjection;

  float weightLogLinear;
  std::vector<float> slicePlaneDistances;
  


  void frustumSliceZDistances(Camera &camera);
  void frustumSliceCorners(Camera &camera, float near, float far,
                                   glm::vec4 (&cornersOut)[8]);
  void frustumSliceBoundsSphere(const glm::vec3 &center,
                                const glm::vec4 (&frustumCorners)[8],
                                glm::vec3 &minBounds,
                                glm::vec3 &maxBounds);
  void frustumSliceBoundsBox(const glm::vec4 (&frustumCorners)[8],
                             const glm::mat4 &view,
                             glm::vec3 &minBounds,
                             glm::vec3 &maxBounds);
  void projectionTexelOffset(const glm::mat4 &viewMatrix,
                             glm::mat4 &projectionMatrix);

  void reset();
};