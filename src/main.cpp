#include "common.h"

// clang-format off
static const float CubeVertices[] = {
  // positions          // texture coords
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
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

static const float QuadVertices[] = {
  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top right
  1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
  -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
}; // top left

static const unsigned int QuadIndices[] = { 0, 1, 3, 1, 2, 3 };

// clang-format off
float PlaneVertices[] = {
  // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
   5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
   5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
  -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
  -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
};
// clang-format on

static const unsigned int PlaneIndices[] = { 0, 1, 3, 1, 2, 3 };

static glm::vec3 LightPos = glm::vec3(1.2f, 1.0f, 2.0f);

enum DEBUG_IDS
{
  DEBUG_CUBES,
  DEBUG_FLOOR,
};

struct DrawResources
{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture1;
  unsigned int texture2;
  unsigned int texture3;
  unsigned int NumTriangles;
};

DrawResources
SetupCubeResources(unsigned int NumVertices,
                   const float Vertices[],
                   unsigned int NumIndices,
                   const unsigned int Indices[])
{
  int aPosLocation = 0;
  int aPosSize = 3;
  int aTexCoordLocation = 1;
  int aTexCoordSize = 2;
  int stride = aPosSize + aTexCoordSize;

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               stride * NumVertices * sizeof(Vertices[0]),
               Vertices,
               GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               NumIndices * sizeof(Indices[0]),
               Indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(aPosLocation,
                        aPosSize,
                        GL_FLOAT,
                        GL_FALSE,
                        stride * sizeof(Vertices[0]),
                        (void*)0);
  glEnableVertexAttribArray(aPosLocation);
  glVertexAttribPointer(aTexCoordLocation,
                        aTexCoordSize,
                        GL_FLOAT,
                        GL_FALSE,
                        stride * sizeof(Vertices[0]),
                        (void*)(aPosSize * sizeof(Vertices[0])));
  glEnableVertexAttribArray(aTexCoordLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources Resources = {};
  Resources.VAO = VAO;
  Resources.VBO = VBO;
  Resources.EBO = EBO;
  Resources.NumTriangles = NumVertices;
  return Resources;
}

void
DrawCube(DrawResources Resources, Shader* CubeShader, glm::mat4 ModelMatrix)
{
  CubeShader->use();
  CubeShader->setMat4("model", ModelMatrix);

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

  glDrawElements(GL_TRIANGLES, Resources.NumTriangles, GL_UNSIGNED_INT, 0);
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
  DrawResources CubeResources;
  DrawResources PlaneResources;
  Shader* DepthTestingShader;
  bool OutlineCubes;

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
    if (glfwGetKey(Window, GLFW_KEY_O) == GLFW_PRESS) {
      OutlineCubes = !OutlineCubes;
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

  void DrawAllTheCubes(DrawResources Resources, Shader* Shader)
  {
    glm::vec3 CubePositions[] = {
      glm::vec3(-1.0f, 0.0f, -1.0f),
      glm::vec3(2.0f, 0.0f, 0.0f),
    };

    for (glm::vec3 CubePosition : CubePositions) {
      glm::mat4 ModelMatrix = glm::mat4(1.0f);
      ModelMatrix = glm::translate(ModelMatrix, CubePosition);
      DrawCube(Resources, Shader, ModelMatrix);
    }
  }

  void Render()
  {
    glm::mat4 PerspectiveProjectionMatrix = glm::perspective(
      glm::radians(Camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    glm::mat4 ViewMatrix = Camera.ViewMatrix();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DepthTestingShader->use();
    DepthTestingShader->setMat4("projection", PerspectiveProjectionMatrix);
    DepthTestingShader->setMat4("view", ViewMatrix);

    {
      glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, DEBUG_CUBES, -1, "Cubes");
      DrawAllTheCubes(CubeResources, DepthTestingShader);
      glPopDebugGroup();
    }

    {
      glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, DEBUG_FLOOR, -1, "Floor");
      glm::mat4 ModelMatrix = glm::mat4(1.0f);
      DrawCube(PlaneResources, DepthTestingShader, ModelMatrix);
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

  int ScreenWidth = 1920;
  int ScreenHeight = 1080;
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
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  {
    int NumAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &NumAttributes);
    std::cout << "Max number of vertex attributes supported: " << NumAttributes
              << std::endl;
  }

  Shader DepthTestingShader("../src/shaders/depth_testing.vert",
                            "../src/shaders/depth_testing.frag");
  DrawResources CubeResources =
    SetupCubeResources(24, CubeVertices, 36, CubeIndices);
  LoadTexture(GL_TEXTURE0, &CubeResources.texture1, "../assets/marble.jpg");
  DrawResources PlaneResources =
    SetupCubeResources(20, PlaneVertices, 6, PlaneIndices);
  LoadTexture(GL_TEXTURE0, &PlaneResources.texture1, "../assets/metal.jpg");

  stbi_set_flip_vertically_on_load(true);

  GlobalWindowState.Window = Window;
  GlobalWindowState.FirstMouseMove = true;
  GlobalWindowState.MouseCurrentFrame = {};
  GlobalWindowState.MouseLastFrame = {};
  GlobalWindowState.MouseCurrentFrame.X = ScreenWidth / 2;
  GlobalWindowState.MouseCurrentFrame.Y = ScreenHeight / 2;
  GlobalWindowState.Camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
  GlobalWindowState.CubeResources = CubeResources;
  GlobalWindowState.PlaneResources = PlaneResources;
  GlobalWindowState.DepthTestingShader = &DepthTestingShader;
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

  CleanupDrawResources(CubeResources);
  CleanupDrawResources(PlaneResources);

  glfwTerminate();
  return 0;
}
