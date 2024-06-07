#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "Texture.h"

#include "Scene.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include <cstdlib>
#include <iostream>

#define OPENGL_VERSION 3.3


int WIDTH = 1920;
int HEIGHT = 1080;

int GLFW_CURSOR_STATUS = 0;

// TODO: This should be its own I/O handler class.
void processInput(GLFWwindow* window, Camera &camera, float deltaTime)
{
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // Enable/disable curser for menu use.
  if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
  {
    if (GLFW_CURSOR_STATUS)
    {
      GLFW_CURSOR_STATUS = 0;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
      GLFW_CURSOR_STATUS = 1;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  // Crude Camera controls.
  // Call this callback function every time the cursor moves.
  double currentCursorPosX, currentCursorPosY;
  glfwGetCursorPos(window, &currentCursorPosX, &currentCursorPosY);
  camera.applyMouseInput((float)currentCursorPosX, (float)currentCursorPosY, GLFW_CURSOR_STATUS);

  // Use keybord input to move in all 6 directions.
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.applyKeybordInput(0, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.applyKeybordInput(1, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.applyKeybordInput(2, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.applyKeybordInput(3, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.applyKeybordInput(4, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    camera.applyKeybordInput(5, deltaTime);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
  glViewport(0, 0, width, height);
}


int main(void)
{
  GLFWwindow* window;

  // Initialize the GLFW library.
  if (!glfwInit())
  {
    std::cerr << "ERROR::main could not start GLFW3" << std::endl;
    return -1;
  }

  // Set the OpenGL version.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and its OpenGL context.
  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLEngine", NULL, NULL);
  if (!window)
  {
    std::cerr << "ERROR::main could not open window with GLFW3" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Make the window's context current.
  glfwMakeContextCurrent(window);

  // Enable automatic window resizing.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Disable cursor.
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  // Initialise the GLAD library.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "ERROR::main Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Get OpenGL version info and render hardware.
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "INFO::main Renderer: " << renderer << std::endl;
  std::cout << "INFO::main OpenGL version supported " << version << std::endl;

  // Enable depth testing 
  glEnable(GL_DEPTH_TEST);

  // Set OpenGL rendering mode.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Shaders.
  Shader shaderBlinn("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\blinn.vert",
                     "D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\blinn.frag");
  
  // Build the scene.
  Scene scene;

  // Camera.
  double cursorPosX, cursorPosY;
  glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
  scene.camera.position = glm::vec3(0.0f, 0.0f, -10.0f);

  // Fixes the camera jump when starting up by setting the curser positions
  // without moving the camera.
  scene.camera.applyMouseInput((float)cursorPosX, (float)cursorPosY, true);

  // Models.
  const char* backpackPath = "D:\\Documents\\Cpp\\OpenGLEngine\\res\\backpack\\backpack.obj";
  scene.loadModel(backpackPath);

  // Lights.
  Light pointLight;
  pointLight.position = glm::vec3(0.0f, 0.0f, 20.0f);
  pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
  pointLight.intensity = 200.0f;
  scene.lights.push_back(pointLight);

  // Set timers.
  glfwSetTime(0.0f);
  float deltaTime = 0.0f;
  float lastTimeFrame = glfwGetTime();
  
  // This is the main rendering loop; it loops until the window is closed.
  while (!glfwWindowShouldClose(window))
  {
    // Calculate the time since last frame.
    deltaTime = (float)(glfwGetTime() - lastTimeFrame);
    lastTimeFrame = glfwGetTime();

    // Detect and manage input.
    processInput(window, scene.camera, deltaTime);

    // Update state of objects.
    // Update camera.
    scene.camera.updateView();

    // Update lights.
    scene.lights[0].position.x = 15.0f * sin(glfwGetTime());
    scene.lights[0].position.z = 15.0f * cos(glfwGetTime());
    for (Light& light : scene.lights)
    {
      light.update(scene.camera);
    }

    // Update objects.


    // Start rendering.
    // Clear buffers and set background colour.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Render the scene.
    Shader& shader = shaderBlinn;
    shader.setUniformMatrix4f("projection", scene.camera.projection);

    for (const Instance& instance : scene.instances)
    {
      const Mesh& mesh = scene.meshes.get(instance.meshID);
      const Material& material = scene.materials.get(instance.materialID);

      // Set Shader info.
      shader.use();
      
      // Materials.
      // TODO: Fixed shader layout should make passing the shader unnecessary.
      material.activate(shader);
      
      // Setup vertex data.
      glBindVertexArray(mesh.vao);

      for (const Light& light : scene.lights)
      {
        // Lights.
        shader.setUniformVec3f("lightPosition", light.getPositionView());
        shader.setUniformVec3f("lightColor", light.color);
        shader.setUniformFloat("lightIntensity", light.intensity);

        for (const unsigned int transformID : instance.transformIDs)        {

        // MVP matrices.
        // TODO: This should be replaced by instance drawing.
        Transform transform = scene.transforms.get(transformID);
        shader.setUniformMatrix4f("modelView", scene.camera.view *
                                                transform.getModelTransform());

        // Draw.
        glDrawElements(GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_INT, 0);
        }
      }
    }
    glBindVertexArray(0);

    // Swap front and back buffers.
    glfwSwapBuffers(window);

    // Poll for and process events.
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}
