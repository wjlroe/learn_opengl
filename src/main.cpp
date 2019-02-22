#include "common.h"

static const float QuadVertices[] = {
  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top right
  1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
  -1.0f, 1.0f,  0.0f, 0.0f, 1.0f
}; // top left

static const unsigned int QuadIndices[] = { 0, 1, 3, 1, 2, 3 };

// clang-format off
static const float CubeVertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
};
// clang-format on

static const unsigned int CubeIndices[] = {
  0,  1,  2,  2,  3,  0,  // top
  4,  5,  6,  6,  7,  4,  // bottom
  8,  9,  10, 10, 11, 8,  // right
  12, 13, 14, 14, 15, 12, // left
  16, 17, 18, 18, 19, 16, // front
  20, 21, 22, 22, 23, 20, // back
};

static glm::vec3 LightPos = glm::vec3(1.2f, 1.0f, 2.0f);

enum DEBUG_IDS
{
  DEBUG_CUBES,
  DEBUG_LIGHT_SOURCE,
};

struct DrawResources
{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture1;
  unsigned int texture2;
  unsigned int texture3;
  struct Shader* Shader;
};

void
DrawRectangle(DrawResources Resources,
              glm::mat4 ViewMatrix,
              glm::mat4 ProjectionMatrix)
{
  Shader* Shader = Resources.Shader;

  Shader->use();
  Shader->setMat4("view", ViewMatrix);
  Shader->setMat4("projection", ProjectionMatrix);

  if (Resources.texture1) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Resources.texture1);
  }
  if (Resources.texture2) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Resources.texture2);
  }
  if (Resources.texture3) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, Resources.texture3);
  }

  glBindVertexArray(Resources.VAO);

  // Draw in wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  int NumTriangles = 6;
  glDrawElements(GL_TRIANGLES, NumTriangles, GL_UNSIGNED_INT, 0);
}

void
DrawCube(DrawResources Resources)
{
  Shader* SimpleShader = Resources.Shader;

  SimpleShader->use();

  if (Resources.texture1) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Resources.texture1);
  }
  if (Resources.texture2) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Resources.texture2);
  }
  if (Resources.texture3) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, Resources.texture3);
  }

  glBindVertexArray(Resources.VAO);

  int NumTriangles = 36;
  glDrawElements(GL_TRIANGLES, NumTriangles, GL_UNSIGNED_INT, 0);
}

struct MouseState
{
  double X;
  double Y;
};

struct MouseScrollState
{
  double XOffset;
  double YOffset;
};

struct WindowState
{
  GLFWwindow* Window;
  int Width;
  int Height;
  struct Camera Camera;
  bool FirstMouseMove;
  MouseState MouseLastFrame;
  MouseState MouseCurrentFrame;
  struct MouseScrollState MouseScrollState;
  DrawResources LampResources;
  DrawResources LightingResources;
  glm::mat4 ProjectionMatrix;

  void ProcessInput(float DeltaTime)
  {
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(Window, true);
    }

    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
      Camera.ProcessKeyboard(FORWARD, DeltaTime);
    }
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
      Camera.ProcessKeyboard(BACKWARD, DeltaTime);
    }
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
      Camera.ProcessKeyboard(LEFT, DeltaTime);
    }
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
      Camera.ProcessKeyboard(RIGHT, DeltaTime);
    }

    MouseLastFrame.X = MouseCurrentFrame.X;
    MouseLastFrame.Y = MouseCurrentFrame.Y;
    glfwGetCursorPos(Window, &MouseCurrentFrame.X, &MouseCurrentFrame.Y);

    if (FirstMouseMove) {
      MouseLastFrame.X = MouseCurrentFrame.X;
      MouseLastFrame.Y = MouseCurrentFrame.Y;
      FirstMouseMove = false;
    }

    float XOffset = (float)MouseCurrentFrame.X - (float)MouseLastFrame.X;
    float YOffset = (float)MouseLastFrame.Y - (float)MouseCurrentFrame.Y;

    Camera.ProcessMouseMovement(XOffset, YOffset);
    Camera.ProcessMouseScroll((float)MouseScrollState.YOffset);
  }

  void DrawAllTheCubes(DrawResources Resources,
                       glm::mat4 ViewMatrix,
                       glm::mat4 ProjectionMatrix,
                       glm::vec3 pointLightPositions[])
  {
    Shader* Shader = Resources.Shader;
    Shader->use();
    Shader->setMat4("view", ViewMatrix);
    Shader->setMat4("projection", ProjectionMatrix);
    Shader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    Shader->setVec3("viewPos", Camera.Position);
    Shader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    Shader->setInt("material.diffuse", 0);
    Shader->setInt("material.specular", 1);
    // Shader->setInt("material.emission", 2);
    Shader->setFloat("material.shininess", 32.0f);

    // directional light
    Shader->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    Shader->setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    Shader->setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    Shader->setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    // point light 1
    Shader->setVec3("pointLights[0].position", pointLightPositions[0]);
    Shader->setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    Shader->setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    Shader->setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Shader->setFloat("pointLights[0].constant", 1.0f);
    Shader->setFloat("pointLights[0].linear", 0.09f);
    Shader->setFloat("pointLights[0].quadratic", 0.032f);

    // point light 2
    Shader->setVec3("pointLights[1].position", pointLightPositions[1]);
    Shader->setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    Shader->setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    Shader->setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Shader->setFloat("pointLights[1].constant", 1.0f);
    Shader->setFloat("pointLights[1].linear", 0.09f);
    Shader->setFloat("pointLights[1].quadratic", 0.032f);

    // point light 3
    Shader->setVec3("pointLights[2].position", pointLightPositions[2]);
    Shader->setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    Shader->setVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    Shader->setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Shader->setFloat("pointLights[2].constant", 1.0f);
    Shader->setFloat("pointLights[2].linear", 0.09f);
    Shader->setFloat("pointLights[2].quadratic", 0.032f);

    // point light 4
    Shader->setVec3("pointLights[3].position", pointLightPositions[3]);
    Shader->setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    Shader->setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    Shader->setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Shader->setFloat("pointLights[3].constant", 1.0f);
    Shader->setFloat("pointLights[3].linear", 0.09f);
    Shader->setFloat("pointLights[3].quadratic", 0.032f);

    // spotlight
    Shader->setVec3("spotLight.position", Camera.Position);
    Shader->setVec3("spotLight.direction", Camera.Front);
    Shader->setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    Shader->setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    Shader->setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Shader->setFloat("spotLight.constant", 1.0f);
    Shader->setFloat("spotLight.linear", 0.09f);
    Shader->setFloat("spotLight.quadratic", 0.032f);
    Shader->setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    Shader->setFloat("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));

    glm::vec3 CubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    int i = 0;
    for (glm::vec3 CubePosition : CubePositions) {
      glm::mat4 ModelMatrix = glm::mat4(1.0f);
      ModelMatrix = glm::translate(ModelMatrix, CubePosition);
      float Angle = 20.0f * i;
      ModelMatrix = glm::rotate(
        ModelMatrix, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
      Shader->setMat4("model", ModelMatrix);
      DrawCube(Resources);
      i++;
    }
  }

  void DrawLightSource(DrawResources Resources,
                       glm::mat4 ViewMatrix,
                       glm::mat4 ProjectionMatrix,
                       glm::vec3 Position)
  {
    Shader* Shader = Resources.Shader;
    Shader->use();
    Shader->setMat4("view", ViewMatrix);
    Shader->setMat4("projection", ProjectionMatrix);
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, Position);
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.2f));
    Shader->setMat4("model", ModelMatrix);
    DrawCube(Resources);
  }

  void Render()
  {
    // glm::mat4 IdentityMatrix = glm::mat4(1.0);
    glm::mat4 PerspectiveProjectionMatrix = glm::perspective(
      glm::radians(Camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    // glm::mat4 OrthoProjectionMatrix = glm::ortho(-10.0f, 10.0f,
    // -10.0f, 10.0f, 0.1f, 100.0f);
    glm::mat4 ViewMatrix = Camera.ViewMatrix();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f),
      glm::vec3(0.0f, 0.0f, -3.0f),
    };

    {
      glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, DEBUG_CUBES, -1, "A Cube");
      DrawAllTheCubes(LightingResources,
                      ViewMatrix,
                      PerspectiveProjectionMatrix,
                      pointLightPositions);
      glPopDebugGroup();
    }

    {
      glPushDebugGroup(
        GL_DEBUG_SOURCE_APPLICATION, DEBUG_LIGHT_SOURCE, -1, "LightSource");
      for (int i = 0; i < 4; i++) {
        glm::vec3 pointLight = pointLightPositions[i];
        DrawLightSource(
          LampResources, ViewMatrix, PerspectiveProjectionMatrix, pointLight);
      }
      glPopDebugGroup();
    }

    glfwSwapBuffers(Window);
  }
};

const static bool CAPTURE_MOUSE = true;
static WindowState GlobalWindowState;

void
ScrollCallback(GLFWwindow* Window, double XOffset, double YOffset)
{
  GlobalWindowState.MouseScrollState.XOffset = XOffset;
  GlobalWindowState.MouseScrollState.YOffset = YOffset;
}

void
FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
  GlobalWindowState.Width = Width;
  GlobalWindowState.Height = Height;
  glViewport(0, 0, GlobalWindowState.Width, GlobalWindowState.Height);
  GlobalWindowState.Render();
}

void
WindowRefreshCallback(GLFWwindow* Window)
{
  GlobalWindowState.Render();
}

void
ErrorCallback(int Error, const char* Description)
{
  std::cout << Description << std::endl;
}

DrawResources
SetupLightResources(Shader* Shader)
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

  int aPosLocation = 0;
  int aPosSize = 3;

  glVertexAttribPointer(
    aPosLocation, aPosSize, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(aPosLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources Resources = {};
  Resources.VAO = VAO;
  Resources.VBO = VBO;
  Resources.EBO = EBO;
  Resources.Shader = Shader;
  return Resources;
}

DrawResources
SetupCubeResources(Shader* Shader)
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

  int aPosLocation = 0;
  int aPosSize = 3;
  int aNormalLocation = 1;
  int aNormalSize = 3;
  int aTexCoordLocation = 2;
  int aTexCoordSize = 2;

  glVertexAttribPointer(
    aPosLocation, aPosSize, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(aPosLocation);
  glVertexAttribPointer(aNormalLocation,
                        aNormalSize,
                        GL_FLOAT,
                        GL_FALSE,
                        8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(aNormalLocation);
  glVertexAttribPointer(aTexCoordLocation,
                        aTexCoordSize,
                        GL_FLOAT,
                        GL_FALSE,
                        8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(aTexCoordLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources Resources = {};
  Resources.VAO = VAO;
  Resources.VBO = VBO;
  Resources.EBO = EBO;
  Resources.Shader = Shader;
  return Resources;
}

void
LoadTexture(unsigned int TextureUnit,
            unsigned int* Texture,
            const char* TextureName)
{
  int Width, Height, NumChannels;
  unsigned char* Data =
    stbi_load(TextureName, &Width, &Height, &NumChannels, 0);
  if (Data) {
    glActiveTexture(TextureUnit);
    glGenTextures(1, Texture);
    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int Format = GL_RGB;
    if (NumChannels == 4) {
      Format = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 Width,
                 Height,
                 0,
                 Format,
                 GL_UNSIGNED_BYTE,
                 Data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(Data);
}

void
CleanupDrawResources(DrawResources Resources)
{
  glDeleteVertexArrays(1, &Resources.VAO);
  glDeleteBuffers(1, &Resources.VBO);
  glDeleteBuffers(1, &Resources.EBO);
}

int
main()
{
  glfwInit();
  glfwSetErrorCallback(ErrorCallback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  int ScreenWidth = 800;
  int ScreenHeight = 600;
  GLFWwindow* Window =
    glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
  if (Window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(Window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  glfwSetScrollCallback(Window, ScrollCallback);
  glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
  glfwSetWindowRefreshCallback(Window, WindowRefreshCallback);
  if (CAPTURE_MOUSE) {
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  glEnable(GL_DEPTH_TEST);

  {
    int NumAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &NumAttributes);
    std::cout << "Max number of vertex attributes supported: " << NumAttributes
              << std::endl;
  }

  Shader LightingShader("../src/shaders/lighting.vert",
                        "../src/shaders/lighting.frag");
  Shader LampShader("../src/shaders/lamp.vert", "../src/shaders/lamp.frag");

  stbi_set_flip_vertically_on_load(true);

  DrawResources LampResources = SetupLightResources(&LampShader);
  DrawResources LightingResources = SetupCubeResources(&LightingShader);

  // FIXME: these aren't related to the DrawResources I don't think...
  LoadTexture(
    GL_TEXTURE0, &LightingResources.texture1, "../assets/container2.png");
  LoadTexture(GL_TEXTURE1,
              &LightingResources.texture2,
              "../assets/container2_specular.png");
  // LoadTexture(GL_TEXTURE2, &LightingResources.texture3,
  // "../assets/matrix.jpg");

  GlobalWindowState.Window = Window;
  GlobalWindowState.FirstMouseMove = true;
  GlobalWindowState.MouseCurrentFrame = {};
  GlobalWindowState.MouseLastFrame = {};
  GlobalWindowState.MouseCurrentFrame.X = ScreenWidth / 2;
  GlobalWindowState.MouseCurrentFrame.Y = ScreenHeight / 2;
  GlobalWindowState.Camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  GlobalWindowState.LampResources = LampResources;
  GlobalWindowState.LightingResources = LightingResources;
  glfwGetFramebufferSize(
    Window, &GlobalWindowState.Width, &GlobalWindowState.Height);

  float DeltaTime = 0.0f;
  float LastFrame = 0.0f;

  while (!glfwWindowShouldClose(Window)) {
    float CurrentFrame = (float)glfwGetTime();
    DeltaTime = CurrentFrame - LastFrame;
    LastFrame = CurrentFrame;

    GlobalWindowState.ProcessInput(DeltaTime);

    GlobalWindowState.Render();
    glfwPollEvents();
  }

  CleanupDrawResources(LightingResources);
  CleanupDrawResources(LampResources);

  glfwTerminate();
  return 0;
}
