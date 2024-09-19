#include "Camera.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "ScreenQuad.h"
#include "Shader.h"
#include "Shadow.h"
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

  // TODO: All of the initialisation should be in a function.
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

  // All the init code until here.

  // Shaders.
  Shader shaderBlinn("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\blinn.vert",
                     "D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\blinn.frag");
  Shader shaderScreen("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\screenTexture.vert",
                      "D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\screenTexture.frag");
  Shader shaderDepth;
  shaderDepth.loadVertexShader("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\depth.vert");
  shaderDepth.loadFragmentShader("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\depth.frag");
  shaderDepth.loadGeometryShader("D:\\Documents\\Cpp\\OpenGLEngine\\res\\shaders\\depth.geo");
  shaderDepth.compileShaderProgram();
  
  // Build the scene.
  Scene scene;

  // Camera.
  double cursorPosX, cursorPosY;
  glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
  scene.camera.position = glm::vec3(0.0f, 0.0f, 0.0f);

  // Fixes the camera jump when starting up by setting the curser positions
  // without moving the camera.
  scene.camera.applyMouseInput((float)cursorPosX, (float)cursorPosY, true);

  // Models.
  const char* sponza = "D:\\Documents\\Cpp\\glTF-Sample-Assets\\Models\\Sponza\\glTF\\Sponza.gltf";
  scene.loadModel(sponza);

  // Lights.
  Light sun;
  sun.type = LightType::directional;
  sun.direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.5f));
  sun.color = glm::vec3(1.0f, 0.8f, 0.4f);
  sun.intensity = 80.0f;
  scene.lights.push_back(sun);

  Light spotLight;
  spotLight.type = LightType::spot;
  spotLight.position = glm::vec3(-100.0f, 20.0f, 0.0f);
  spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
  spotLight.intensity = 200.0f;
  spotLight.spotlightAngle = 50.0f;
  scene.lights.push_back(spotLight);
  for (int i=0; i<2; ++i)
  {
    spotLight.position.x += 100.0f;
    scene.lights.push_back(spotLight);
  }

  // Shadow pass framebuffer and texture.
  // Generate texture buffer to store shadow pass depth data.

  // Value of 4 is hard coded everywhere! Changing it here won't work!
  int nShadowCascadeSlices = 4;
  
  const unsigned int depthMapWidth =  4096;
  const unsigned int depthMapHeight = 4096;

  Texture depthMapTexture;
  depthMapTexture.emptyTexture3D(depthMapWidth, depthMapHeight, nShadowCascadeSlices,
                                 GL_DEPTH_COMPONENT, GL_FLOAT);
  depthMapTexture.minMagFilter3D(GL_NEAREST, GL_NEAREST);
  depthMapTexture.wrapMode3D(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

  // Generate framebuffer to render to.
  FrameBuffer depthMapFBO;

  // Bind texture to framebuffer.
  depthMapFBO.bindTexture3D(GL_DEPTH_ATTACHMENT, depthMapTexture.getId());

  // Set the colour buffer to GL_NONE.
  depthMapFBO.setDrawBuffer(GL_NONE);
  depthMapFBO.setReadBuffer(GL_NONE);

  // Check if the framebuffer was setup correctly.
  depthMapFBO.checkStatus();

  // Generate quad that covers screen to render textures to.
  ScreenQuad screenQuad;
  screenQuad.generate(0.5f, 1.0f, 0.5f, 1.0f);

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

    // Update objects.

    // Pre-render setup.
    // Build UBO for lights
    int numLights = static_cast<int>(scene.lights.size());

    // Store all of the lights in a UBO
    unsigned int lightsUBO;
    glGenBuffers(1, &lightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);

    // Link buffer to shader block, the uniform binding point.
    unsigned int blockIndex = glGetUniformBlockIndex(shaderBlinn.shaderId, "LightsBlock");
    glUniformBlockBinding(shaderBlinn.shaderId, blockIndex, 0);

    // Set size of buffer.
    int numLightsByteSize = 16;
    int lightStructByteSize = 64;
    int bufferByteSize = numLightsByteSize + lightStructByteSize * numLights;
    glBufferData(GL_UNIFORM_BUFFER, bufferByteSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 
    
    // Define the range of the buffer that links to a uniform binding point.
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, lightsUBO, 0, bufferByteSize);

    // Fill buffer with data.
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    numLightsByteSize, &numLights);
    glBufferSubData(GL_UNIFORM_BUFFER, numLightsByteSize, 
                    lightStructByteSize * numLights, &scene.lights[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);

    // Start rendering.

    // Shadow pass.
    // Calculate the 8 corners of the camera frustum in world space.
    Shadow shadow = Shadow(depthMapWidth, depthMapHeight, nShadowCascadeSlices);

    float sunHeight = 1000.0f;
    shadow.shadowMatrices(sunHeight, sun.direction, scene.camera);

    // Configure shader and matrices.
    shaderDepth.use();
    for (int slice=0; slice<nShadowCascadeSlices; ++slice)
    {
      shaderDepth.setUniformMatrix4f("lightViewProjection["+std::to_string(slice)+"]",
                                     shadow.getViewProjection(slice));
    }

    // Set viewport and framebuffer.
    glViewport(0, 0, depthMapWidth, depthMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO.getId());
    glClear(GL_DEPTH_BUFFER_BIT);

    // Render the scene shadows.
    for (const Instance& instance : scene.instances)
    {
      // Setup vertex data.
      const Mesh& mesh = scene.meshes.get(instance.meshID);
      glBindVertexArray(mesh.vao);

      for (const unsigned int transformID : instance.transformIDs)
      {
        // MVP matrices.
        // TODO: This should be replaced by instance drawing.
        Transform transform = scene.transforms.get(transformID);
        shaderDepth.setUniformMatrix4f("model", transform.getModelTransform());

        // Draw.
        glDrawElements(GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_INT, 0);
      }
    }
    glBindVertexArray(0);

    // Main render pass.
    // Rebind default framebuffer and screen viewport.
    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear buffers and set background colour.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Render the scene.
    // Set Shader info.
    Shader& shader = shaderBlinn;
    shader.use();

    // Set matrices.
    shader.setUniformMatrix4f("projection", scene.camera.projection);
    shader.setUniformMatrix4f("view", scene.camera.view);

    // Light matrices and depth texture for shadow mapping.
    for (int slice=0; slice<nShadowCascadeSlices; ++slice)
    {
      // Read slice+1 to skip the near plane, e.g. only include far planes of
      // each cascade slice.
      shader.setUniformFloat("slicePlaneDistances["+std::to_string(slice)+"]",
                                shadow.getSlicePlaneDistances(slice+1));
    }

    for (int slice=0; slice<nShadowCascadeSlices; ++slice)
    {
      shader.setUniformMatrix4f("lightViewProjection["+std::to_string(slice)+"]",
                                shadow.getViewProjection(slice));
    }

    shader.setUniformInt("textureDepth", 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMapTexture.getId());

    for (const Instance& instance : scene.instances)
    {
      const Mesh& mesh = scene.meshes.get(instance.meshID);
      const Material& material = scene.materials.get(instance.materialID);
      
      // Materials.
      // TODO: Fixed shader layout should make passing the shader unnecessary.
      material.activate(shader);
      
      // Setup vertex data.
      glBindVertexArray(mesh.vao);

      for (const unsigned int transformID : instance.transformIDs)
      {
        // MVP matrices.
        // TODO: This should be replaced by instance drawing.
        Transform transform = scene.transforms.get(transformID);
        shader.setUniformMatrix4f("modelView", scene.camera.view *
                                              transform.getModelTransform());
        shader.setUniformMatrix4f("model", transform.getModelTransform());

        // Draw.
        glDrawElements(GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_INT, 0);
      }
    }
    glBindVertexArray(0);

    // Render depth buffer
    // glViewport(0, 0, WIDTH, HEIGHT);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); 
    
    // // Configure shader and matrices.
    // shaderScreen.use();
    // shaderScreen.setUniformInt("screenTexture", 0);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D_ARRAY, depthMapTexture.getId());

    // // Render texture to quad.
    // glDisable(GL_DEPTH_TEST);
    // glBindVertexArray(screenQuad.vao);
    // glDrawElements(GL_TRIANGLES, screenQuad.nIndices, GL_UNSIGNED_INT, 0);
    // glEnable(GL_DEPTH_TEST);
    // glBindVertexArray(0);

    // Swap front and back buffers.
    glfwSwapBuffers(window);

    // Poll for and process events.
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}
