#include "common.h"

static const float QuadVertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f}; // top left

static const unsigned int QuadIndices[] = {
    0, 1, 3,
    1, 2, 3};

// clang-format off
static const float CubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
};
// clang-format on

static const unsigned int CubeIndices[] = {
    0, 1, 2, 2, 3, 0,       // top
    4, 5, 6, 6, 7, 4,       // bottom
    8, 9, 10, 10, 11, 8,    // right
    12, 13, 14, 14, 15, 12, // left
    16, 17, 18, 18, 19, 16, // front
    20, 21, 22, 22, 23, 20, // back
};

static const float CAMERA_SPEED_MULTIPLIER = 2.5f;

struct DrawResources
{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture1;
  unsigned int texture2;
};

struct CameraState
{
  float Yaw;
  float Pitch;
  float FOV;
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
};

struct MouseState
{
  double X;
  double Y;
};

struct WindowState
{
  int Width;
  int Height;
  struct CameraState CameraState;
  bool FirstMouseMove;
  MouseState MouseLastFrame;
  MouseState MouseCurrentFrame;
};

struct MouseScrollState
{
  double XOffset;
  double YOffset;
};

static struct MouseScrollState MouseScrollState = {};

void ScrollCallback(GLFWwindow *Window, double XOffset, double YOffset)
{
  MouseScrollState.XOffset = XOffset;
  MouseScrollState.YOffset = YOffset;
}

void ErrorCallback(int Error, const char *Description)
{
  std::cout << Description << std::endl;
}

void ProcessInput(GLFWwindow *Window, WindowState *WindowState, float DeltaTime)
{
  if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(Window, true);
  }

  float CameraSpeed = CAMERA_SPEED_MULTIPLIER * DeltaTime;

  CameraState *Camera = &WindowState->CameraState;

  if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
  {
    Camera->Position += CameraSpeed * Camera->Front;
  }

  if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
  {
    Camera->Position -= CameraSpeed * Camera->Front;
  }

  if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
  {
    Camera->Position -= glm::normalize(glm::cross(Camera->Front, Camera->Up)) * CameraSpeed;
  }

  if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
  {
    Camera->Position += glm::normalize(glm::cross(Camera->Front, Camera->Up)) * CameraSpeed;
  }

  WindowState->MouseLastFrame.X = WindowState->MouseCurrentFrame.X;
  WindowState->MouseLastFrame.Y = WindowState->MouseCurrentFrame.Y;
  glfwGetCursorPos(Window, &WindowState->MouseCurrentFrame.X, &WindowState->MouseCurrentFrame.Y);

  if (WindowState->FirstMouseMove)
  {
    WindowState->MouseLastFrame.X = WindowState->MouseCurrentFrame.X;
    WindowState->MouseLastFrame.Y = WindowState->MouseCurrentFrame.Y;
    WindowState->FirstMouseMove = false;
  }

  float XOffset = WindowState->MouseCurrentFrame.X - WindowState->MouseLastFrame.X;
  float YOffset = WindowState->MouseLastFrame.Y - WindowState->MouseCurrentFrame.Y;

  float MouseSensitivity = 0.05f;
  XOffset *= MouseSensitivity;
  YOffset *= MouseSensitivity;

  Camera->Yaw += XOffset;
  Camera->Pitch += YOffset;

  if (Camera->Pitch > 89.0f)
  {
    Camera->Pitch = 89.0f;
  }
  if (Camera->Pitch < -89.0f)
  {
    Camera->Pitch = -89.0f;
  }

  if (Camera->FOV >= 1.0f && Camera->FOV <= 45.0f)
  {
    Camera->FOV -= MouseScrollState.YOffset;
  }
  if (Camera->FOV <= 1.0f)
  {
    Camera->FOV = 1.0f;
  }
  if (Camera->FOV >= 45.0f)
  {
    Camera->FOV = 45.0f;
  }
}

DrawResources SetupQuadResources()
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadIndices), QuadIndices, GL_STATIC_DRAW);

  int aPosLocation = 0;
  int aPosSize = 3;
  int aTexLocation = 1;
  int aTexSize = 2;

  glVertexAttribPointer(aPosLocation, aPosSize, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(aPosLocation);
  glVertexAttribPointer(aTexLocation, aTexSize, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(aTexLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources Resources = {};
  Resources.VAO = VAO;
  Resources.VBO = VBO;
  Resources.EBO = EBO;
  return Resources;
}

DrawResources SetupCubeResources()
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

  int aPosLocation = 0;
  int aPosSize = 3;
  int aTexLocation = 1;
  int aTexSize = 2;

  glVertexAttribPointer(aPosLocation, aPosSize, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(aPosLocation);
  glVertexAttribPointer(aTexLocation, aTexSize, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(aTexLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources Resources = {};
  Resources.VAO = VAO;
  Resources.VBO = VBO;
  Resources.EBO = EBO;
  return Resources;
}

void LoadTexture(unsigned int TextureUnit, unsigned int *Texture, const char *TextureName)
{
  int Width, Height, NumChannels;
  unsigned char *Data = stbi_load(TextureName, &Width, &Height, &NumChannels, 0);
  if (Data)
  {
    glActiveTexture(TextureUnit);
    glGenTextures(1, Texture);
    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int Format = GL_RGB;
    if (NumChannels == 4)
    {
      Format = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(Data);
}

void DrawRectangle(Shader SimpleShader, DrawResources Resources)
{
  SimpleShader.use();

  if (Resources.texture1)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Resources.texture1);
  }
  if (Resources.texture2)
  {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Resources.texture2);
  }

  glBindVertexArray(Resources.VAO);

  // Draw in wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  int NumTriangles = 6;
  glDrawElements(GL_TRIANGLES, NumTriangles, GL_UNSIGNED_INT, 0);
}

void DrawCube(Shader SimpleShader, DrawResources Resources)
{
  SimpleShader.use();

  if (Resources.texture1)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Resources.texture1);
  }
  if (Resources.texture2)
  {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Resources.texture2);
  }

  glBindVertexArray(Resources.VAO);

  int NumTriangles = 36;
  glDrawElements(GL_TRIANGLES, NumTriangles, GL_UNSIGNED_INT, 0);
}

void Render(Shader Shader, DrawResources QuadResources, DrawResources CubeResources)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Shader.use();
  // DrawRectangle(Shader, QuadResources);
  glm::vec3 CubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)};
  int i = 0;
  for (glm::vec3 CubePosition : CubePositions)
  {
    float Angle = 20.0f * i;
    if (i % 2 == 0)
    {
      Angle += glfwGetTime() * 25.0f;
    }
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, CubePosition);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
    Shader.setMat4("model", ModelMatrix);
    DrawCube(Shader, CubeResources);
    ++i;
  }
}

void CleanupDrawResources(DrawResources Resources)
{
  glDeleteVertexArrays(1, &Resources.VAO);
  glDeleteBuffers(1, &Resources.VBO);
  glDeleteBuffers(1, &Resources.EBO);
}

int main()
{
  glfwInit();
  glfwSetErrorCallback(ErrorCallback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  int ScreenWidth = 800;
  int ScreenHeight = 600;
  GLFWwindow *Window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
  if (Window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(Window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetScrollCallback(Window, ScrollCallback);
  glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);

  {
    int NumAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &NumAttributes);
    std::cout << "Max number of vertex attributes supported: " << NumAttributes << std::endl;
  }

  Shader SimpleShader("../src/shaders/simple.vert", "../src/shaders/simple.frag");

  stbi_set_flip_vertically_on_load(true);

  DrawResources QuadResources = SetupQuadResources();
  LoadTexture(GL_TEXTURE0, &QuadResources.texture1, "../assets/container.jpg");
  LoadTexture(GL_TEXTURE1, &QuadResources.texture2, "../assets/awesomeface.png");

  DrawResources CubeResources = SetupCubeResources();
  // FIXME: these aren't related to the DrawResources I don't think...
  LoadTexture(GL_TEXTURE0, &CubeResources.texture1, "../assets/container.jpg");
  LoadTexture(GL_TEXTURE1, &CubeResources.texture2, "../assets/awesomeface.png");

  SimpleShader.use();
  SimpleShader.setInt("texture1", 0);
  SimpleShader.setInt("texture2", 1);

  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;

  WindowState WindowState = {};
  WindowState.FirstMouseMove = true;
  WindowState.MouseCurrentFrame = {};
  WindowState.MouseLastFrame = {};
  WindowState.MouseCurrentFrame.X = ScreenWidth / 2;
  WindowState.MouseCurrentFrame.Y = ScreenHeight / 2;
  {
    CameraState CameraState = {};
    CameraState.FOV = 45.0f;
    CameraState.Position = glm::vec3(0.0f, 0.0f, 3.0f);
    CameraState.Front = glm::vec3(0.0f, 0.0f, -1.0f);
    CameraState.Up = glm::vec3(0.0f, 1.0f, 0.0f);
    WindowState.CameraState = CameraState;
  }

  float DeltaTime = 0.0f;
  float LastFrame = 0.0f;

  while (!glfwWindowShouldClose(Window))
  {
    float CurrentFrame = glfwGetTime();
    DeltaTime = CurrentFrame - LastFrame;
    LastFrame = CurrentFrame;

    {
      WindowState.CameraState.Front.x =
          cos(glm::radians(WindowState.CameraState.Pitch)) * cos(glm::radians(WindowState.CameraState.Yaw));
      WindowState.CameraState.Front.y = sin(glm::radians(WindowState.CameraState.Pitch));
      WindowState.CameraState.Front.z =
          cos(glm::radians(WindowState.CameraState.Pitch)) * sin(glm::radians(WindowState.CameraState.Yaw));
      WindowState.CameraState.Front = glm::normalize(WindowState.CameraState.Front);
      ViewMatrix = glm::lookAt(
          WindowState.CameraState.Position,
          WindowState.CameraState.Position + WindowState.CameraState.Front,
          WindowState.CameraState.Up);

      SimpleShader.use();
      SimpleShader.setMat4("view", ViewMatrix);
    }

    glfwGetFramebufferSize(Window, &WindowState.Width, &WindowState.Height);
    {
      SimpleShader.use();
      ProjectionMatrix = glm::perspective(glm::radians(WindowState.CameraState.FOV), (float)WindowState.Width / (float)WindowState.Height, 0.1f, 100.0f);
      SimpleShader.setMat4("projection", ProjectionMatrix);

      glViewport(0, 0, WindowState.Width, WindowState.Height);
    }

    ProcessInput(Window, &WindowState, DeltaTime);
    Render(SimpleShader, QuadResources, CubeResources);
    glfwSwapBuffers(Window);
    glfwPollEvents();
  }

  CleanupDrawResources(CubeResources);

  glfwTerminate();
  return 0;
}
