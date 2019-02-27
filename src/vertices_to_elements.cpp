#include <vector>

// clang-format off
static const float CubeVerticesToConvert[] = {
  // Back face
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left // 0
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right   // 1
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right // 2
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right    // 1
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left  // 0
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left     // 3
  // Front face
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
  // Left face
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
  // Right face
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
  // Bottom face
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
  // Top face
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
};
// clang-format on

void
genCubeIndices()
{
  std::vector<std::vector<float>> vertices;
  int stride = 5;
  int verticeCount =
    sizeof(CubeVerticesToConvert) / sizeof(CubeVerticesToConvert[0]);
  for (int i = 0; i < verticeCount; i += stride) {
    std::vector<float> vertice;
    for (int j = 0; j < stride; j++) {
      vertice.push_back(CubeVerticesToConvert[i + j]);
    }
    vertices.push_back(vertice);
  }

  // now we have a vector of all the "elements"
  std::vector<int> indices;
  std::vector<std::vector<float>> nonDuplicated;
  for (auto vertice : vertices) {
    std::vector<std::vector<float>>::iterator it =
      std::find(nonDuplicated.begin(), nonDuplicated.end(), vertice);
    if (it != nonDuplicated.end()) {
      indices.push_back(std::distance(nonDuplicated.begin(), it));
    } else {
      nonDuplicated.push_back(vertice);
      indices.push_back(nonDuplicated.size() - 1);
    }
  }

  printf("static const float CubeVertices[] = {\n");
  for (auto vertice : nonDuplicated) {
    for (auto num : vertice) {
      printf(" % 1.1ff,", num);
    }
    printf("\n");
  }
  printf("};");
  printf("\n\n");
  printf("static const unsigned int CubeIndices[] = {\n");

  for (int i = 0; i < indices.size(); i++) {
    if ((i > 0) && (i % 6 == 0)) {
      printf("\n");
    }
    printf("  %2d,", indices[i]);
  }
  printf("\n};");
  printf("\n\n");
}
