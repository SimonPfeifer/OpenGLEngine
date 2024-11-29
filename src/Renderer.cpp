#include <iostream>

#include "glad/glad.h"

#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Window.h"

Renderer::Renderer()
{
  // Enable depth testing 
  glEnable(GL_DEPTH_TEST);

  // Set OpenGL rendering mode.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Generate buffers
  glGenBuffers(1, &m_lightsUBO);
  
  // TODO: Shaders should probably not be hard coded. Maybe include as part of
  //       the scene in their own database. "Ubershader" would only require one.
  // Load shader.
  m_shader.load("..\\..\\res\\shaders\\blinn.vert",
                "..\\..\\res\\shaders\\blinn.frag");
}

void Renderer::render(const Scene& scene, const Window& window) const
{
  // Main render pass.
  // Rebind default framebuffer and screen viewport.
  updateViewportSize(window);
  resetFramebuffer();

  // Render the scene.
  // Set Shaders.
  m_shader.use();

  // Set matrices.
  m_shader.setUniformMatrix4f("projection", scene.camera.getProjectionMatrix());
  m_shader.setUniformMatrix4f("view", scene.camera.getViewMatrix());

  // Bind ligths buffer.
  buildLightsUBO(scene.lights);
  glBindBuffer(GL_UNIFORM_BUFFER, m_lightsUBO);

  for (const Instance& instance : scene.instances)
  {
    const Mesh& mesh = scene.meshes.get(instance.meshID);
    const Material& material = scene.materials.get(instance.materialID);
    
    // Materials.
    // TODO: Fixed shader layout should make passing the material unnecessary.
    m_shader.bindMaterial(material);
    
    // Setup vertex data.
    mesh.bind();

    for (const unsigned int transformID : instance.transformIDs)
    {
      // TODO: This should be replaced by instance drawing.
      Transform transform = scene.transforms.get(transformID);
      m_shader.setUniformMatrix4f("modelView", scene.camera.getViewMatrix() *
                                  transform.getModelTransform());
      m_shader.setUniformMatrix4f("model", transform.getModelTransform());

      // Draw.
      glDrawElements(GL_TRIANGLES, mesh.getNIndices(), GL_UNSIGNED_INT, 0);
    }
  }

  // Cleanup.
  glBindVertexArray(0);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::resetFramebuffer() const
 {
  // Bind default frambuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Clear buffers and set background colour.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
 }

void Renderer::updateViewportSize(const Window& window) const
{
  // Change the size of the viewport to match the window size.
  int frameWidth = 0;
  int frameHeight = 0;
  window.getSize(frameWidth, frameHeight);
  glViewport(0, 0, frameWidth, frameHeight);
}

void Renderer::buildLightsUBO(const std::vector<Light>& lights) const
{
  // Store all of the lights in a UBO
  glBindBuffer(GL_UNIFORM_BUFFER, m_lightsUBO);

  // TODO: Should link the buffer to a shader block but only one buffer exists
  // so default is fine for now.
  // unsigned int blockIndex = glGetUniformBlockIndex(shader.shaderId, "LightsBlock");
  // glUniformBlockBinding(shader.shaderId, blockIndex, 0);

  // Set size of buffer.
  int numLightsByteSize = 16;
  int lightStructByteSize = 64;
  int numLights = static_cast<int>(lights.size());
  int bufferByteSize = numLightsByteSize + lightStructByteSize * numLights;
  glBufferData(GL_UNIFORM_BUFFER, bufferByteSize, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0); 
  
  // Define the range of the buffer that links to a uniform binding point.
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_lightsUBO, 0, bufferByteSize);

  // Fill buffer with data.
  glBindBuffer(GL_UNIFORM_BUFFER, m_lightsUBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0,
                  numLightsByteSize, &numLights);
  glBufferSubData(GL_UNIFORM_BUFFER, numLightsByteSize, 
                  lightStructByteSize * numLights, &lights[0]);
  glBindBuffer(GL_UNIFORM_BUFFER, 0); 
}
