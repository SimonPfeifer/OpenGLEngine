#include "Shadow.h"

#include <glm/gtc/matrix_transform.hpp>

Shadow::Shadow(int mapWidht, int mapHeight, int nCascadeSlices)
{
  nShadowCascadeSlices = nCascadeSlices;
  shadowMapWidth = mapWidht;
  shadowMapHeight = mapHeight;

  view = new glm::mat4[nShadowCascadeSlices];
  projection = new glm::mat4[nShadowCascadeSlices];
  viewProjection = new glm::mat4[nShadowCascadeSlices];
}

Shadow::~Shadow()
{
  delete[] view;
  delete[] projection;
  delete[] viewProjection;
}

void Shadow::shadowMatrices(float lightHeight, glm::vec3 &lightDirection,
                            Camera &camera)
{
  // Calculate the distance of the slice Z planes, including
  // camera near and far.
  float *slicePlaneDistances = new float[nShadowCascadeSlices+1];
  frustumSliceZDistances(camera, slicePlaneDistances);

  for (int slice=0; slice<nShadowCascadeSlices; ++slice)
  {
    // Get the corners of the camera frustum slice.
    float sliceNear = slicePlaneDistances[slice];
    float sliceFar = slicePlaneDistances[slice+1];
    glm::vec4 frustumCorners[8];
    frustumSliceCorners(camera, sliceNear, sliceFar, frustumCorners);

    // Calculate the geometric center of the slice frustum.
    glm::vec3 center = glm::vec3(0.0f);
    for (const auto& v : frustumCorners)
    {
        center += glm::vec3(v);
    }
    center /= 8.0f; // number of corners

    // Calculate the bounds for the light projection matrix.
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    // Calculate the bounds using a tight box around the frustum slice in the
    // light view space. Produces shadow crawling when the camera rotates due to
    // the bounding box changing size.
    // view[slice] = glm::lookAt(center - lightDirection,
    //                           center,
    //                           glm::vec3(0.0f, 1.0f, 0.0f));
    // Shadow::frustumSliceBoundsBox(frustumCorners, view[slice],
    //                               minBounds, maxBounds);

    // Calculate the light projection bounds using a sphere to keep the
    // bounds constant independent of camera rotation. Removes shadow 
    // crawling when the camera rotates to look around.
    Shadow::frustumSliceBoundsSphere(center, frustumCorners,
                                     minBounds, maxBounds);

    // Add the light height to the z bounds to add more depth "behind"
    // the light projection. This allow for shadow casters to exist outside
    // the camera frustum up to this distance in the light direction.
    maxBounds.z += lightHeight;
    glm::vec3 extentBounds = maxBounds - minBounds;

    // Generate the view and projection matrix.
    // Put the light at the maximum bound in the z direction.
    view[slice] = glm::lookAt(center - lightDirection*maxBounds.z,
                              center,
                              glm::vec3(0.0f, 1.0f, 0.0f));
    projection[slice] = glm::ortho(minBounds.x, maxBounds.x,
                                   minBounds.y, maxBounds.y,
                                   0.0f, extentBounds.z);

    // Force the light porjection to move in shadow map texel increments
    // to remove shadow crawling when the camera moves.
    projectionTexelOffset(view[slice], projection[slice]);

    viewProjection[slice] = projection[slice] * view[slice];
  }

  // Clean up.
  delete[] slicePlaneDistances;
}

void Shadow::frustumSliceZDistances(Camera &camera, float distances[])
{
  // Calculate the distance of the slice Z planes, including
  // camera near and far.
  float near = camera.getNear();
  float far = camera.getFar();
  float zRange = far - near;

  // Linear spacing.
  float dz = zRange / (float)nShadowCascadeSlices;
  
  for (int i=0; i<nShadowCascadeSlices+1; ++i)
  {
    distances[i] = near + dz * (float)i;
  }
}

void Shadow::frustumSliceCorners(Camera &camera, float near, float far,
                                 glm::vec4 (&cornersOut)[8])
{
  // Frustum corners in normalised device coordinate.
  glm::vec4 frustumCornersNDC[8] = 
  {
    glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), // near bottom left
    glm::vec4( 1.0f, -1.0f, -1.0f, 1.0f), // near bottom right
    glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f), // near top left
    glm::vec4( 1.0f,  1.0f, -1.0f, 1.0f), // near top right

    glm::vec4(-1.0f, -1.0f,  1.0f, 1.0f), // and the far equivalent
    glm::vec4( 1.0f, -1.0f,  1.0f, 1.0f),
    glm::vec4(-1.0f,  1.0f,  1.0f, 1.0f),
    glm::vec4( 1.0f,  1.0f,  1.0f, 1.0f),
  };

  glm::mat4 cameraProjection = glm::perspective(glm::radians(camera.getFOV()),
                                                camera.getAspectRatio(),
                                                near, far);
  glm::mat4 cameraViewProjectionInverse = glm::inverse(cameraProjection *
                                                       camera.view);

  for (int i=0; i<8; ++i)
  {
    cornersOut[i] = cameraViewProjectionInverse * frustumCornersNDC[i];
    cornersOut[i] = cornersOut[i] / cornersOut[i].w;
  }
}

void Shadow::frustumSliceBoundsSphere(const glm::vec3 &center,
                                      const glm::vec4 (&frustumCorners)[8],
                                      glm::vec3 &minBounds,
                                      glm::vec3 &maxBounds)
{
  // Find the maximum size sphere required to fit the entire camera, 
  // frustum centered on the camera frustum. This allows the light
  // projection to be of constant size independent of camera rotation.
  float radius = 0.0f;
  for (int i=0; i<8; ++i)
  {
    float cornerDistance = glm::length(center - 
                                        glm::vec3(frustumCorners[i]));
    radius = std::max(radius, cornerDistance);
  }

  maxBounds = glm::vec3(radius, radius, radius);
  minBounds = -maxBounds;
}

void Shadow::frustumSliceBoundsBox(const glm::vec4 (&frustumCorners)[8],
                                   const glm::mat4 &view,
                                   glm::vec3 &minBounds,
                                   glm::vec3 &maxBounds)
{
  // Find the bounds by fitting a tight box around the frustum corners in the 
  // light view space.
  minBounds = glm::vec3(std::numeric_limits<float>::max());
  maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
  for (int i=0; i<8; ++i)
  {
    glm::vec3 cornerView = glm::vec3(view * frustumCorners[i]);
    minBounds = glm::min(minBounds, cornerView);
    maxBounds = glm::max(maxBounds, cornerView);
  }

}

void Shadow::projectionTexelOffset(const glm::mat4 &viewMatrix,
                                   glm::mat4 &projectionMatrix)
{
  // Offset the light projection to keep the shadow map moving in
  // texel increments. Stops shadow crawling when the camera moves;
  // it does remove crawling when camera is rotation.

  // Project the world origin into light NDC.
  glm::mat4 shadowViewProjection = projectionMatrix * viewMatrix;
  glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  origin = shadowViewProjection * origin;

  // Convert NDC into texel space. NDC covers [-1,1], so a unit [0,1]
  // covers half the shadow map size.
  origin.x *=  shadowMapWidth / 2.0f;
  origin.y *=  shadowMapHeight / 2.0f;

  // The offset is the fractional part of the textel coordinates.
  // Warning: as the camera moves, the light projection bounds change 
  // slightly, bouncing between texel coords causing slight shadow
  // flickering (rounding error?).
  glm::vec4 originRounded = glm::round(origin);
  glm::vec4 offset = origin - originRounded;

  // Convert back to NDC. Only interested in X and Y direction.
  offset.x *= 2.0f / shadowMapWidth;
  offset.y *= 2.0f / shadowMapHeight;
  offset.z = 0.0f;
  offset.w = 0.0f;

  projectionMatrix[3] -= offset;
}