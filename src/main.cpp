#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const char *vertexShaderSource = "#version 330 core \n\
layout (location = 0) in vec3 aPos;\n\
\n\
out vec4 vertexColor;\n\
\n\
void main() \n\
{ \n\
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n\
  vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n\
} \n\
";

static const char *fragmentShaderSource = "#version 330 core\n\
out vec4 FragColor;\n\
\n\
in vec4 vertexColor;\n\
\n\
void main()\n\
{\n\
  FragColor = vertexColor;\n\
}\n\
";

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
      0.5f, 0.5f, 0.0f,   // top right
      0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f}; // top left
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

  glVertexAttribPointer(aPosLocation, aPosSize, GL_FLOAT, GL_FALSE, aPosSize * sizeof(float), (void *)0);
  glEnableVertexAttribArray(aPosLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  DrawResources resources = {};
  resources.VAO = VAO;
  resources.VBO = VBO;
  resources.EBO = EBO;
  return resources;
}

void drawRectangle(unsigned int shaderProgram, unsigned int VAO)
{
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  // Draw in wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  int numTriangles = 6;
  glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);
}

void render(unsigned int shaderProgram, unsigned int VAO)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  drawRectangle(shaderProgram, VAO);
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

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  DrawResources resources = setupDrawResources();

  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    render(shaderProgram, resources.VAO);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanupDrawResources(resources);

  glfwTerminate();
  return 0;
}
