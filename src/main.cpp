#include "common.h"

struct DrawResources
{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture1;
  unsigned int texture2;
};

struct WindowState
{
  int Width;
  int Height;
};

void ErrorCallback(int Error, const char *Description)
{
  std::cout << Description << std::endl;
}

void ProcessInput(GLFWwindow *Window)
{
  if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(Window, true);
  }
}

DrawResources SetupDrawResources()
{
  float Vertices[] = {
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f}; // top left
  unsigned int Indices[] = {
      0, 1, 3,
      1, 2, 3};

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

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

  {
    glm::mat4 TranslationMatrix = glm::mat4(1.0f);
    TranslationMatrix = glm::translate(TranslationMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
    TranslationMatrix = glm::rotate(TranslationMatrix, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
    SimpleShader.setMat4("transform", TranslationMatrix);

    int NumTriangles = 6;
    glDrawElements(GL_TRIANGLES, NumTriangles, GL_UNSIGNED_INT, 0);
  }

  {
    glm::mat4 TranslationMatrix = glm::mat4(1.0f);
    TranslationMatrix = glm::translate(TranslationMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
    float ScaleFactor = abs(sin(glfwGetTime()));
    TranslationMatrix = glm::scale(TranslationMatrix, glm::vec3(ScaleFactor, ScaleFactor, 0.0f));
    SimpleShader.setMat4("transform", TranslationMatrix);

    int NumTriangles = 6;
    glDrawElements(GL_TRIANGLES, NumTriangles, GL_UNSIGNED_INT, 0);
  }
}

void Render(Shader shader, DrawResources Resources)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  DrawRectangle(shader, Resources);
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

  GLFWwindow *Window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

  {
    int NumAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &NumAttributes);
    std::cout << "Max number of vertex attributes supported: " << NumAttributes << std::endl;
  }

  glViewport(0, 0, 800, 600);

  Shader SimpleShader("../src/shaders/simple.vert", "../src/shaders/simple.frag");

  stbi_set_flip_vertically_on_load(true);

  DrawResources Resources = SetupDrawResources();
  LoadTexture(GL_TEXTURE0, &Resources.texture1, "../assets/container.jpg");
  LoadTexture(GL_TEXTURE1, &Resources.texture2, "../assets/awesomeface.png");

  SimpleShader.use();
  SimpleShader.setInt("texture1", 0);
  SimpleShader.setInt("texture2", 1);

  WindowState WindowState = {};
  int WindowWidth, WindowHeight;

  while (!glfwWindowShouldClose(Window))
  {
    glfwGetFramebufferSize(Window, &WindowWidth, &WindowHeight);
    if ((WindowWidth != WindowState.Width) || (WindowHeight != WindowState.Height))
    {
      glViewport(0, 0, WindowWidth, WindowHeight);
      WindowState.Width = WindowWidth;
      WindowState.Height = WindowHeight;
    }

    ProcessInput(Window);
    Render(SimpleShader, Resources);
    glfwSwapBuffers(Window);
    glfwPollEvents();
  }

  CleanupDrawResources(Resources);

  glfwTerminate();
  return 0;
}
