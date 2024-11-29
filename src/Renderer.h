#pragma once

#include "glad/glad.h"

#include "Scene.h"
#include "Shader.h"
#include "Window.h"

/**
 * @brief The renderer class manages the rendering of a given scene to a window.
 */
class Renderer
{
public:
  Renderer();
  ~Renderer() = default;

  Renderer(const Renderer& other) = delete;
  Renderer& operator=(const Renderer& other) = delete;

  /**
   * @brief Renders the scene to the window.
   * 
   * @param scene The scene object containing the geometry, materials and lights.
   * @param window The window object which will display the render output.
   */
  void render(const Scene& scene, const Window& window) const;

private:
  /** The shader used to render the scene.*/
  Shader m_shader{};

  /**
   * @brief Reset the framebuffer to default and clear appropriate buffers.
   */
  void resetFramebuffer() const;

  /**
   * @brief Set the size of the viewport to the size of the window.
   * 
   * @param window 
   */
  void updateViewportSize(const Window& window) const;

  /** Buffer object containing all lights in the scene */
  GLuint m_lightsUBO{};

  /**
   * @brief Creates a buffer for the lights and streams the data into it.
   * 
   * @param lights The scene lights.
   */
  void buildLightsUBO(const std::vector<Light>& lights) const;
};