#include "common.h"

#ifdef GEN_CONVERT_ELEMENTS_FROM_VERTICES
#include "vertices_to_elements.cpp"
#endif

// clang-format off
static const float CubeVertices[] = {
 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
 -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
};

static const unsigned int CubeIndices[] = {
   0,   1,   2,   1,   0,   3,
   4,   5,   6,   6,   7,   4,
   8,   9,  10,  10,   4,   8,
  11,  12,   1,  12,  11,  13,
  10,  14,   5,   5,   4,  10,
   3,  11,   1,  11,   3,  15,
};
// clang-format on

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
  DEBUG_WINDOW,
  DEBUG_CONTROLLER,
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
InitDrawResources(unsigned int NumVertices,
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
  Resources.NumTriangles = NumIndices;
  return Resources;
}

void
RenderDrawResources(DrawResources Resources, Shader* CubeShader, glm::mat4 ModelMatrix)
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
  int WindowWidth;
  int WindowHeight;
  bool Maximized;
  bool pboSupported;
};

struct ScribbleTexture
{
  GLubyte* Data;
  int Width;
  int Height;
  int NumChannels;
  GLenum PixelFormat;
  DrawResources Resources;
};

ScribbleTexture
InitScribbleTexture(int Width, int Height, int NumChannels, GLenum PixelFormat)
{
  ScribbleTexture Result = {};
  Result.Data = new GLubyte[Width * Height * NumChannels];
  memset(Result.Data, 0, Width * Height * NumChannels);
  Result.Width = Width;
  Result.Height = Height;
  Result.NumChannels = NumChannels;
  Result.PixelFormat = PixelFormat;
  Result.Resources = InitDrawResources(20, QuadVertices, 6, QuadIndices);

  glGenTextures(1, &Result.Resources.texture1);
  glBindTexture(GL_TEXTURE_2D, Result.Resources.texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Result.Width, Result.Height, 0, Result.PixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)Result.Data);
  glBindTexture(GL_TEXTURE_2D, 0);

  int ColorSize = sizeof(int);
  std::cout << "size of Color: " << ColorSize << std::endl;

  return Result;
}

void PaintIntoScribble(ScribbleTexture* Texture)
{
  static int Color = 0xffffffff;

  if (!Texture->Data) {
    return;
  }

  int* ptr = (int*)Texture->Data;

  for (int i = 0; i < Texture->Height; i++) {
    for (int j = 0; j < Texture->Width; j++) {
      *ptr = Color;
      ++ptr;
    }
  }
}

void RenderScribble(ScribbleTexture* Texture, Shader* Shader, glm::mat4 ModelMatrix)
{
  // non-PBO way to do this...
  glBindTexture(GL_TEXTURE_2D, Texture->Resources.texture1);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Texture->Width, Texture->Height, Texture->PixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)Texture->Data);
  PaintIntoScribble(Texture);

  // actually do the texture with a shader thingy...
  RenderDrawResources(Texture->Resources, Shader, ModelMatrix);
}

#define NUM_CONTROLLERS 32
#define MAX_HATS 32
#define MAX_AXISES 32
#define MAX_BUTTONS 64

enum ControllerType {
  DisconnectedController,
  ControllerGamepad,
  ControllerJoystick,
};

struct ControllerState {
  const char* Name;
  ControllerType Type;
  int NumHats;
  unsigned char Hats[MAX_HATS];
  int NumAxises;
  float Axises[MAX_AXISES];
  int NumButtons;
  unsigned char Buttons[MAX_BUTTONS];
};

struct GameState
{
  GLFWwindow* Window;
  struct WindowState WindowState;
  int Width;
  int Height;
  struct Camera Camera;
  bool FirstMouseMove;
  MouseState MouseLastFrame;
  MouseState MouseCurrentFrame;
  struct MouseScrollState MouseScrollState;
  DrawResources CubeResources;
  DrawResources PlaneResources;
  DrawResources WindowResources;
  DrawResources ControllerResources;
  Shader* DepthTestingShader;
  bool OutlineCubes;
  bool ShowEditor;
  ControllerState Controllers[NUM_CONTROLLERS];
  ScribbleTexture ControllerTexture;

  void ProcessJoystick(int JoystickNum) {
    ControllerState* Controller = &Controllers[JoystickNum];
    if (glfwJoystickIsGamepad(JoystickNum)) {
      Controller->Name = glfwGetGamepadName(JoystickNum);
      Controller->Type = ControllerGamepad;
    } else {
      Controller->Name = glfwGetJoystickName(JoystickNum);
      Controller->Type = ControllerJoystick;

      const float* Axes = glfwGetJoystickAxes(JoystickNum, &Controller->NumAxises);
      for (int i = 0; i < Controller->NumAxises; i++) {
        Controller->Axises[i] = Axes[i];
      }

      const unsigned char* Hats = glfwGetJoystickHats(JoystickNum, &Controller->NumHats);
      for (int HatIdx = 0; HatIdx < Controller->NumHats; HatIdx++) {
        Controller->Hats[HatIdx] = Hats[HatIdx];
      }

      const unsigned char* Buttons = glfwGetJoystickButtons(JoystickNum, &Controller->NumButtons);
      for (int ButtonIdx = 0; ButtonIdx < Controller->NumButtons; ButtonIdx++) {
        Controller->Buttons[ButtonIdx] = Buttons[ButtonIdx];
      }
    }
  }

  void ProcessJoysticks(float DeltaTime) {
    for (int JoystickNum = GLFW_JOYSTICK_1; JoystickNum < GLFW_JOYSTICK_16 + 1;
         JoystickNum++) {
      // TODO: remove disconnected joysticks/controllers
      if (glfwJoystickPresent(JoystickNum)) {
        ProcessJoystick(JoystickNum);
      }
    }
  }

  void ProcessInput(float DeltaTime)
  {
    if (!ShowEditor) {
      // TODO: move keyboard stuff to ProcessKeyInput
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

    ProcessJoysticks(DeltaTime);
  }

  void ProcessKeyInput(int Key, int Scancode, int Action, int Mods, int LastKey)
  {
    if ((Key == GLFW_KEY_Q) && (Action == GLFW_PRESS) && (LastKey != Key) &&
        (Mods & GLFW_MOD_CONTROL)) {
      glfwSetWindowShouldClose(Window, true);
    }

    if ((Key == GLFW_KEY_F1) && (Action == GLFW_PRESS) && (LastKey != Key)) {
      ToggleEditor();
    }

    if (ShowEditor) {
      if ((Key == GLFW_KEY_ESCAPE) && (Action == GLFW_PRESS) &&
          (LastKey != Key)) {
        ToggleEditor();
      }
    } else {
      if ((Key == GLFW_KEY_ESCAPE) && (Action == GLFW_PRESS) &&
          (LastKey != Key)) {
        glfwSetWindowShouldClose(Window, true);
      }
    }
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
      RenderDrawResources(Resources, Shader, ModelMatrix);
    }
  }

  void ToggleEditor()
  {
    ShowEditor = !ShowEditor;
    if (ShowEditor) {
      // unbind mouse capture
      glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
      // rebind mouse capture
      glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
      glEnable(GL_CULL_FACE);
      DrawAllTheCubes(CubeResources, DepthTestingShader);
      glDisable(GL_CULL_FACE);
      glPopDebugGroup();
    }

    {
      glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, DEBUG_FLOOR, -1, "Floor");
      glm::mat4 ModelMatrix = glm::mat4(1.0f);
      RenderDrawResources(PlaneResources, DepthTestingShader, ModelMatrix);
      glPopDebugGroup();
    }

    {
      glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, DEBUG_WINDOW, -1, "Window");
      float moveUp = 0.5;
      glm::vec3 WindowPositions[] = {
        glm::vec3(-1.5f, moveUp, -0.48f), glm::vec3(1.5f, moveUp, 0.51f),
        glm::vec3(0.0f, moveUp, 0.7f),    glm::vec3(-0.3f, moveUp, -2.3f),
        glm::vec3(0.5f, moveUp, -0.6f),
      };
      std::sort(std::begin(WindowPositions),
                std::end(WindowPositions),
                [this](glm::vec3 a, glm::vec3 b) {
                  float distance_a = glm::length(Camera.Position - a);
                  float distance_b = glm::length(Camera.Position - b);
                  return distance_a > distance_b;
                });
      for (glm::vec3 WindowPosition : WindowPositions) {
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, WindowPosition);
        RenderDrawResources(WindowResources, DepthTestingShader, ModelMatrix);
      }
      glPopDebugGroup();
    }

    {
      glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, DEBUG_CONTROLLER, -1, "Controller");
      glm::mat4 OrthoProjectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
      // TODO: Do we want to "follow" the camera, i.e. reposition the ortho elements relative
      // to the camera; or set a static ortho-camera so ortho elements exist on a separate plane
      DepthTestingShader->setMat4("projection", OrthoProjectionMatrix);
      ViewMatrix = glm::mat4(1.0f);
      DepthTestingShader->setMat4("view", ViewMatrix); // no camera?
      glm::mat4 ModelMatrix = glm::mat4(1.0f);
      ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 20.0f));
      // ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.8f, 0.8f, 1.0f));

      {
        // test the corners of the vertex...
        glm::vec4 top_right    = glm::vec4(QuadVertices[0], QuadVertices[1], QuadVertices[2], 1.0f);
        glm::vec4 bottom_right = glm::vec4(QuadVertices[5], QuadVertices[6], QuadVertices[7], 1.0f);
        glm::vec4 bottom_left  = glm::vec4(QuadVertices[10], QuadVertices[11], QuadVertices[12], 1.0f);

        glm::vec4 model_top_right = ModelMatrix * top_right;
        glm::vec4 view_top_right = ViewMatrix * model_top_right;
        glm::vec4 projection_top_right = OrthoProjectionMatrix * view_top_right;

        glm::vec4 model_bottom_left = ModelMatrix * bottom_left;
        glm::vec4 view_bottom_left = ViewMatrix * model_bottom_left;
        glm::vec4 projection_bottom_left = OrthoProjectionMatrix * view_bottom_left;

        glm::vec4 woot = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
      }
      RenderScribble(&ControllerTexture, DepthTestingShader, ModelMatrix);
      glPopDebugGroup();
    }

    if (ShowEditor) {
      {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool ShowHats[NUM_CONTROLLERS] = {};
        static bool ShowAxises[NUM_CONTROLLERS] = {};
        static bool ShowButtons[NUM_CONTROLLERS] = {};

        if (!ImGui::Begin("Editor", &ShowEditor)) {
        }
        ImGui::BeginChild("Scrolling");
        for (int ControllerNum = 0; ControllerNum < NUM_CONTROLLERS; ControllerNum++) {
          ImGui::PushID(ControllerNum);
          ControllerState Controller = Controllers[ControllerNum];
          if (Controller.Type) {
            char ThisControllerType[512] = "UnknownControllerType";
            switch (Controller.Type) {
              case ControllerGamepad: {
                strcpy(ThisControllerType, "Gamepad");
              } break;
              case ControllerJoystick: {
                strcpy(ThisControllerType, "Joystick");
              } break;
            }
            ImGui::Columns(4);
            ImGui::Text("%04d [%s]: %s", ControllerNum, ThisControllerType, Controller.Name);
            ImGui::NextColumn();
            if (Controller.Type == ControllerJoystick) {
              {
                bool ThisShowHats = ImGui::Button("Hats");
                if (ThisShowHats) {
                  if (!ShowHats[ControllerNum]) {
                    ShowHats[ControllerNum] = true;
                  }
                }
                ThisShowHats = ThisShowHats || ShowHats[ControllerNum];
                if (ThisShowHats) {
                  if (!ImGui::Begin("Hats State"), &ShowHats[ControllerNum]) {
                  }
                  for (int HatIdx = 0; HatIdx < Controller.NumHats; HatIdx++) {
                    ImGui::PushID(HatIdx);
                    unsigned char Hat = Controller.Hats[HatIdx];
                    ImGui::Value("hatidx", HatIdx);
                    ImGui::Value("Centered", Hat == GLFW_HAT_CENTERED);
                    ImGui::Value("Up", (bool)(Hat & GLFW_HAT_UP));
                    ImGui::Value("Right", (bool)(Hat & GLFW_HAT_RIGHT));
                    ImGui::Value("Down", (bool)(Hat & GLFW_HAT_DOWN));
                    ImGui::Value("Left", (bool)(Hat & GLFW_HAT_LEFT));
                    ImGui::PopID();
                  }
                  ImGui::End();
                }
                ImGui::NextColumn();
              }
              {
                bool ThisShowAxises = ImGui::Button("Axises");
                if (ThisShowAxises) {
                  if (!ShowAxises[ControllerNum]) {
                    ShowAxises[ControllerNum] = true;
                  }
                }
                ThisShowAxises = ThisShowAxises || ShowAxises[ControllerNum];
                if (ThisShowAxises) {
                  if (!ImGui::Begin("Axises State"), &ShowAxises[ControllerNum]) {
                  }
                  for (int AxisIdx = 0; AxisIdx < Controller.NumAxises; AxisIdx++) {
                    ImGui::PushID(AxisIdx);
                    ImGui::Value("AxisIdx", AxisIdx);
                    ImGui::Value("value", Controller.Axises[AxisIdx]);
                    ImGui::PopID();
                  }
                  ImGui::End();
                }
                ImGui::NextColumn();
              }
              {
                bool ThisShowButtons = ImGui::Button("Buttons");
                if (ThisShowButtons) {
                  if (!ShowButtons[ControllerNum]) {
                    ShowButtons[ControllerNum] = true;
                  }
                }
                ThisShowButtons = ThisShowButtons || ShowButtons[ControllerNum];
                if (ThisShowButtons) {
                  if (!ImGui::Begin("Buttons State"), &ShowButtons[ControllerNum]) {
                  }
                  for (int ButtonIdx = 0; ButtonIdx < Controller.NumButtons; ButtonIdx++) {
                    ImGui::PushID(ButtonIdx);
                    ImGui::Value("ButtonIdx", ButtonIdx);
                    ImGui::Value("value", Controller.Buttons[ButtonIdx]);
                    ImGui::PopID();
                  }
                  ImGui::End();
                }
              }
            } else {
              ImGui::NextColumn(); // between hats and axis buttons otherwise
              ImGui::NextColumn(); // between axis and buttons buttons otherwise
            }
            ImGui::NextColumn(); // end of the row
            //ImGui::Separator();
          }
          ImGui::PopID();
        }
        ImGui::EndChild();
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      }
    }

    glfwSwapBuffers(Window);
  }
};

const static bool CAPTURE_MOUSE = true;
static GameState GlobalGameState;

void
ScrollCallback(GLFWwindow* Window, double XOffset, double YOffset)
{
  GlobalGameState.MouseScrollState.XOffset = XOffset;
  GlobalGameState.MouseScrollState.YOffset = YOffset;
}

void
FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
  GlobalGameState.Width = Width;
  GlobalGameState.Height = Height;
  glViewport(0, 0, GlobalGameState.Width, GlobalGameState.Height);
  GlobalGameState.Render();
}

void
WindowSizeCallback(GLFWwindow* Window, int Width, int Height)
{
  GlobalGameState.WindowState.WindowWidth = Width;
  GlobalGameState.WindowState.WindowHeight = Height;
  GlobalGameState.WindowState.Maximized =
    glfwGetWindowAttrib(Window, GLFW_MAXIMIZED);
  GlobalGameState.Render();
}

void
KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
  static int LastKey = 0;

  // TODO: translate to own types, hereon in, to GLFW types to be used
  GlobalGameState.ProcessKeyInput(Key, Scancode, Action, Mods, LastKey);

  if ((Action == GLFW_RELEASE) || (Action == GLFW_REPEAT)) {
    LastKey = 0;
  } else {
    LastKey = Key;
  }
}

void
WindowRefreshCallback(GLFWwindow* Window)
{
  GlobalGameState.Render();
}

void
ErrorCallback(int Error, const char* Description)
{
  std::cerr << Description << std::endl;
}

int
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

    unsigned int Format = GL_RGB;
    if (NumChannels == 1) {
      Format = GL_RED;
    } else if (NumChannels == 3) {
      Format = GL_RGB;
    } else if (NumChannels == 4) {
      Format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, *Texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 Format,
                 Width,
                 Height,
                 0,
                 Format,
                 GL_UNSIGNED_BYTE,
                 Data);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (NumChannels == 4) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    std::cout << "Failed to load texture from TextureName: " << TextureName
              << std::endl;
    return -1;
  }
  stbi_image_free(Data);
  return 0;
}

void
CleanupDrawResources(DrawResources Resources)
{
  glDeleteVertexArrays(1, &Resources.VAO);
  glDeleteBuffers(1, &Resources.VBO);
  glDeleteBuffers(1, &Resources.EBO);
}

static char BUILD_DIR[MAX_PATH];

std::string
BuildRelativePath(const char* Path)
{
  std::filesystem::path Result = BUILD_DIR;
  Result.append(Path);

  return Result.string();
}

int
main()
{
#ifdef WIN32
  {
    // Find the dir of this exe, to resolve references relative to build dir
    char result[MAX_PATH];
    int bytes = GetModuleFileName(NULL, result, MAX_PATH);
    if (bytes == 0) {
      std::cout << "Could not work out directory of running executable with "
                   "GetModuleFileName!"
                << std::endl;
      return -1;
    }
    char dirname[MAX_PATH];
    _splitpath(result, NULL, BUILD_DIR, NULL, NULL);
    std::cout << "ok, build dir is: " << BUILD_DIR << std::endl;
  }
#endif

#ifdef GEN_CONVERT_ELEMENTS_FROM_VERTICES
  genCubeIndices();
#endif

  glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
  glfwInit();
  glfwSetErrorCallback(ErrorCallback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

#ifdef __linux
  glfwWindowHintString(GLFW_X11_CLASS_NAME, "Game");
  glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "LearnOpenGL");
#endif

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  struct WindowState WindowState;
  WindowState.WindowWidth = 1920;
  WindowState.WindowHeight = 1080;
  WindowState.Maximized = false;

  glfwWindowHint(GLFW_MAXIMIZED, WindowState.Maximized);
  GLFWwindow* Window = glfwCreateWindow(WindowState.WindowWidth,
                                        WindowState.WindowHeight,
                                        "LearnOpenGL",
                                        NULL,
                                        NULL);
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

  if (glfwExtensionSupported("GL_ARB_pixel_buffer_object")) {
    WindowState.pboSupported = true;
  } else {
    WindowState.pboSupported = false;
  }

  const char* glfwVersion = glfwGetVersionString();
  std::cout << "GLFW version: " << glfwVersion << std::endl;

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(Window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  stbi_set_flip_vertically_on_load(true);

  glfwSetScrollCallback(Window, ScrollCallback);
  glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
  glfwSetWindowSizeCallback(Window, WindowSizeCallback);
  glfwSetWindowRefreshCallback(Window, WindowRefreshCallback);
  glfwSetKeyCallback(Window, KeyCallback);
  if (CAPTURE_MOUSE) {
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  std::cout << "Loading game controller mapping database ";
  char* mappings = LoadFileContents("gamecontrollerdb.txt");
  glfwUpdateGamepadMappings(mappings);
  std::cout << "... DONE" << std::endl;
  free(mappings);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  {
    int NumAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &NumAttributes);
    std::cout << "Max number of vertex attributes supported: " << NumAttributes
              << std::endl;
  }

  Shader DepthTestingShader(
    BuildRelativePath("../src/shaders/depth_testing.vert").c_str(),
    BuildRelativePath("../src/shaders/depth_testing.frag").c_str());
  DrawResources CubeResources =
    InitDrawResources(24, CubeVertices, 36, CubeIndices);
  if (LoadTexture(GL_TEXTURE0,
                  &CubeResources.texture1,
                  BuildRelativePath("../assets/marble.jpg").c_str()) != 0) {
    return -1;
  }
  DrawResources PlaneResources =
    InitDrawResources(20, PlaneVertices, 6, PlaneIndices);
  if (LoadTexture(GL_TEXTURE0,
                  &PlaneResources.texture1,
                  BuildRelativePath("../assets/metal.jpg").c_str()) != 0) {
    return -1;
  }
  DrawResources WindowResources =
    InitDrawResources(20, QuadVertices, 6, QuadIndices);
  if (LoadTexture(GL_TEXTURE0,
                  &WindowResources.texture1,
                  BuildRelativePath("../assets/blending_transparent_window.png")
                    .c_str()) != 0) {
    return -1;
  }

  {
    // try basic quad shaders
    Shader QuadShader(
      BuildRelativePath("../src/shaders/quad_vert.glsl").c_str(),
      BuildRelativePath("../src/shaders/quad_frag.glsl").c_str());
    std::cout << "Done compiling quad shaders" << std::endl;
  }

  ScribbleTexture ControllerTexture = InitScribbleTexture(500, 500, 4, GL_RGBA);

  GlobalGameState.Window = Window;
  GlobalGameState.WindowState = WindowState;
  GlobalGameState.FirstMouseMove = true;
  GlobalGameState.MouseCurrentFrame = {};
  GlobalGameState.MouseLastFrame = {};
  GlobalGameState.MouseCurrentFrame.X = WindowState.WindowWidth / 2;
  GlobalGameState.MouseCurrentFrame.Y = WindowState.WindowHeight / 2;
  GlobalGameState.Camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
  GlobalGameState.CubeResources = CubeResources;
  GlobalGameState.PlaneResources = PlaneResources;
  GlobalGameState.WindowResources = WindowResources;
  GlobalGameState.DepthTestingShader = &DepthTestingShader;
  GlobalGameState.ShowEditor = false;
  GlobalGameState.ControllerTexture = ControllerTexture;
  for (int ControllerIndex = 0; ControllerIndex < NUM_CONTROLLERS; ControllerIndex++) {
    GlobalGameState.Controllers[ControllerIndex] = {};
  }
  glfwGetFramebufferSize(
    Window, &GlobalGameState.Width, &GlobalGameState.Height);

  float DeltaTime = 0.0f;
  float LastFrame = 0.0f;

  while (!glfwWindowShouldClose(Window)) {
    float CurrentFrame = (float)glfwGetTime();
    DeltaTime = CurrentFrame - LastFrame;
    LastFrame = CurrentFrame;

    GlobalGameState.ProcessInput(DeltaTime);

    GlobalGameState.Render();
    glfwPollEvents();
  }

  CleanupDrawResources(CubeResources);
  CleanupDrawResources(PlaneResources);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
