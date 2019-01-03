#include "common.h"

struct DrawResources
{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
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
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f}; // top left
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
  int aColorLocation = 1;
  int aPosSize = 3;
  int aColorSize = 3;

  glVertexAttribPointer(aPosLocation, aPosSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(aPosLocation);
  glVertexAttribPointer(aColorLocation, aColorSize, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(aColorLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources resources = {};
  resources.VAO = VAO;
  resources.VBO = VBO;
  resources.EBO = EBO;
  return resources;
}

void drawRectangle(Shader shader, unsigned int VAO)
{
  float timeValue = glfwGetTime();
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

  shader.use();

  shader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

  glBindVertexArray(VAO);
  // Draw in wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  int numTriangles = 6;
  glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);
}

void render(Shader shader, unsigned int VAO)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  drawRectangle(shader, VAO);
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

  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    render(shader, resources.VAO);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanupDrawResources(resources);

  glfwTerminate();
  return 0;
}
