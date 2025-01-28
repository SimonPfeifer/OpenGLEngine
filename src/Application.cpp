#include <iostream>

// Disable inclusion of the environment header to fix header conflicts of GLFW
// and glad.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Application.h"
#include "Scene.h"

#include "Mac.h"
#include "FluidSimulation.h"
#include "GuiWindowFluidSimulation.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <memory>

// TODO: This should be its own Input handler class.
void processInput(GLFWwindow* window, Camera &camera, float deltaTime)
{
  // Toggle pause state for menu use.
  static int PAUSE_STATE = 0;
  static int escapePress = GLFW_RELEASE;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS &&
      escapePress == GLFW_RELEASE)
  {
    if (PAUSE_STATE == 1)
    {
      PAUSE_STATE = 0;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
      PAUSE_STATE = 1;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
  escapePress = glfwGetKey(window, GLFW_KEY_ESCAPE);
  
  // Crude Camera controls.
  // Call this callback function every time the cursor moves.
  double currentCursorPosX, currentCursorPosY;
  glfwGetCursorPos(window, &currentCursorPosX, &currentCursorPosY);
  camera.applyMouseInput((float)currentCursorPosX, (float)currentCursorPosY, PAUSE_STATE);

  if (!PAUSE_STATE)
  {
    // Close the window with Q.
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Use keybord input to move in all 6 directions.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.applyKeyboardInput(0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.applyKeyboardInput(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.applyKeyboardInput(2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.applyKeyboardInput(3, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      camera.applyKeyboardInput(4, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.applyKeyboardInput(5, deltaTime);
  }
}


Application::Application()
{
  // Initialize the GLFW library.
  if (!glfwInit())
  {
    std::cerr << "ERROR::main could not start GLFW3" << std::endl;
  }

  // Need to create a window and make it current for GLAD to initialize.
  m_window = new Window("Rendering Engine", 1280, 720);
  m_window->setResolution(WindowResolution::r720);
  
  // Initialize the GLAD library.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "ERROR::main Failed to initialize GLAD" << std::endl;
  }

  // Get OpenGL version info and render hardware.
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "INFO::main Renderer: " << renderer << std::endl;
  std::cout << "INFO::main OpenGL version supported " << version << std::endl;

  // Initialize the renderer.
  m_renderer = new Renderer;

  // Initialize ImGui.
  m_gui = new Gui(*m_window);

  // Initialize scene.
  m_scene = new Scene;

  // Set timers.
  glfwSetTime(0.0f);
  m_deltaTime = 0.0f;
  m_lastTimeFrame = glfwGetTime();
}

Application::~Application()
{
  // Shutdown order is the reverse of the initialization order.
  // Shutdown scene.
  delete m_scene;

  // Shutdown GUI.
  delete m_gui;

  // Shutdown renderer.
  delete m_renderer;

  // Shutdown window.
  glfwTerminate();
  delete m_window;

  std::cout << "Shutting down!" << std::endl;
}

void Application::run()
{
  // TODO: Replace manual scene setup by a scene file that can be read,
  //       modified and saved.
  // Setup the scene.
  // Camera.
  m_scene->camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
  double currentCursorPosX, currentCursorPosY;
  glfwGetCursorPos(m_window->getWindowInstance(), &currentCursorPosX, &currentCursorPosY);
  m_scene->camera.applyMouseInput((float)currentCursorPosX, (float)currentCursorPosY, 1);
  m_scene->camera.yaw = 180.0f;
  
  // Models.
  // const char* sponza = "../../../res/assets/sponza/glTF/Sponza.gltf";
  // m_scene->loadModel(sponza, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.05f));


  // Lights.
  Light sun;
  sun.type = LightType::directional;
  sun.direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.5f));
  sun.color = glm::vec3(1.0f, 0.95f, 0.7f);
  sun.intensity = 80.0f;
  m_scene->lights.push_back(sun);

  // Simulation
  float dx = 1.0f;
  int nCellsX = 100;
  int nCellsY = 100;
  FluidSimulation sim(nCellsX, nCellsY, 0.01f);

  Material fluidMaterial;
  unsigned int materialId = m_scene->materials.add();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // std::vector<float> testOutput(nCellsX*nCellsX, 1.0f);
  // m_scene->materials.get(materialId).textureDiffuse.setTextureData(nCellsX, nCellsX, GL_RED,
  //   GL_FLOAT, testOutput.data());
  m_scene->materials.get(materialId).textureDiffuse.setTextureData(nCellsX, nCellsX, GL_RED,
    GL_FLOAT, sim.outputPressure());
  m_scene->materials.get(materialId).textureDiffuse.wrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  m_scene->materials.get(materialId).textureDiffuse.minMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
  m_scene->materials.get(materialId).textureDiffuse.generateMipmap();

  float quadWidth = 10.0f;
  float quadHeight = 10.0f;
  std::vector<Vertex> vertices;
  vertices.emplace_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f),
                        glm::vec2(0.0f, 0.0f)));
  vertices.emplace_back(Vertex(glm::vec3(quadWidth, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f),
                        glm::vec2(1.0f, 0.0f)));
  vertices.emplace_back(Vertex(glm::vec3(0.0f, quadHeight, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f),
                        glm::vec2(0.0f, 1.0f)));
  vertices.emplace_back(Vertex(glm::vec3(quadWidth, quadHeight, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f),
                        glm::vec2(1.0f, 1.0f)));
  std::vector<int> indices{0, 1, 2, 2, 1, 3};
  unsigned int meshId = m_scene->meshes.add();
  m_scene->meshes.get(meshId).loadVertexData(vertices, indices);

  unsigned int transformId = m_scene->transforms.add();
  m_scene->transforms.get(transformId).setPosition(glm::vec3(-quadWidth/2., -quadHeight/2., -10.0f));
  // m_scene->transforms.get(transformId).setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
  std::vector<unsigned int> transformIds{transformId};
  
  m_scene->instances.emplace_back(Instance(meshId, materialId, transformIds));

  // GUI for editing the fluid simulation.
  GuiWindowFluidSimulation guiWindowFluidSimulation{sim};
  m_gui->addWindow(std::make_unique<GuiWindowFluidSimulation>(guiWindowFluidSimulation));

  std::cout << "Running!" << std::endl;
  while (m_window->isAlive())
  {
    // Calculate the time since last frame.
    m_deltaTime = (float)(glfwGetTime() - m_lastTimeFrame);
    m_lastTimeFrame = glfwGetTime();

    // Poll for and process events.
    glfwPollEvents();
    processInput(m_window->getWindowInstance(), m_scene->camera, m_deltaTime);

    // TODO: Scene or a SceneManager should be responsible for updating rather
    //       than doing it manually here.
    // Update state of objects.
    // Update camera.
    m_scene->camera.update();

    // Update lights.

    // Update objects.

    // Update fluid simulation.
    sim.update(m_deltaTime);
    m_scene->materials.get(materialId).textureDiffuse.setTextureData(nCellsX, nCellsX, GL_RED,
      GL_FLOAT, sim.outputSmoke());

    // Main application loop.
    m_renderer->render(*m_scene, *m_window);
    m_gui->render();
    
    // Swap front and back buffer.
    m_window->swapBuffers();
  }
}
