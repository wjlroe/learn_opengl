#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if 0
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad.c"
#include "shader.cpp"
#include "camera.cpp"
