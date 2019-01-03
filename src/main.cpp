#include "common.h"

struct DrawResources
{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture1;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void error_callback(int error, const char *description)
{
  std::cout << description << std::endl;
}

void process_input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

DrawResources setupDrawResources()
{
  float vertices[] = {
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f}; // top left
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

  DrawResources resources = {};
  resources.VAO = VAO;
  resources.VBO = VBO;
  resources.EBO = EBO;
  return resources;
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(Data);
}

void drawRectangle(Shader shader, DrawResources Resources)
{
  float timeValue = glfwGetTime();
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

  shader.use();

  shader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

  if (Resources.texture1)
  {
    glBindTexture(GL_TEXTURE_2D, Resources.texture1);
  }

  glBindVertexArray(Resources.VAO);
  // Draw in wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  int numTriangles = 6;
  glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);
}

void render(Shader shader, DrawResources Resources)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  drawRectangle(shader, Resources);
}

void cleanupDrawResources(DrawResources resources)
{
  glDeleteVertexArrays(1, &resources.VAO);
  glDeleteBuffers(1, &resources.VBO);
  glDeleteBuffers(1, &resources.EBO);
}

int main()
{
  glfwInit();
  glfwSetErrorCallback(error_callback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

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

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader shader("../src/shaders/simple.vert", "../src/shaders/simple.frag");

  DrawResources resources = setupDrawResources();
  LoadTexture(GL_TEXTURE0, &resources.texture1, "../assets/container.jpg");

  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    render(shader, resources);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanupDrawResources(resources);

  glfwTerminate();
  return 0;
}
