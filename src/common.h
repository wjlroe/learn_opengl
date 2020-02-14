// clang-format off
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>
#include <filesystem>
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
#include "glad.c"

#if 0
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// imgui
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui.cpp"
#include "imgui_draw.cpp"
#include "imgui_widgets.cpp"
#include "imgui_internal.h"
#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_glfw.cpp"

#include "simpleini/SimpleIni.h"

#include "camera.cpp"
#include "shader.cpp"
#include "mesh.cpp"
#include "model.cpp"
// clang-format on
