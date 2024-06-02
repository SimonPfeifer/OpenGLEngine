#include "AssetBox.h"
#include "Camera.h"
#include "LightBox.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

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

  // Camera.
  double cursorPosX, cursorPosY;
  glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
  Camera camera(window, (float)cursorPosX, (float)cursorPosY);
  camera.position = glm::vec3(0.0f, 0.0f, -10.0f);

  // Models.
  const char* backpackPath = "D:\\Documents\\Cpp\\OpenGLEngine\\res\\backpack\\backpack.obj";
  Model model(backpackPath);
  model.rotation.y = 180.0f;
  model.scale = glm::vec3(1.5f);
  
  // Shaders.
  Shader shaderBlinn("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\blinn.vert",
                     "D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\blinn.frag");
  
  Shader shaderLight("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\light.vert",
                     "D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\light.frag");
  
  // Lights.
  LightBox light;
  light.position = glm::vec3(0.0f, 0.0f, 20.0f);
  light.color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f);
  light.shader = shaderLight;

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
    processInput(window, camera, deltaTime);

    // Update state of objects.
    // Update camera.
    camera.updateView();

    // Update lights.
    light.position.x = 15.0f * sin(glfwGetTime());
    light.position.z = 15.0f * cos(glfwGetTime());
    light.update(camera);

    // Update objects.
    model.update();

    // Start rendering objects.
    // Clear buffers and set background colour.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Render the lights.    
    light.render(camera);

    // Render the objects.
    model.render(camera, light, shaderBlinn);

    // Swap front and back buffers.
    glfwSwapBuffers(window);

    // Poll for and process events.
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}
