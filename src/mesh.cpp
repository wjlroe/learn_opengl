using namespace std;

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture
{
  unsigned int id;
  string type;
};

class Mesh
{
public:
  // Mesh data
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  // Functions
  Mesh(vector<Vertex> vertices,
       vector<unsigned int> indices,
       vector<Texture> textures)
  {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
  }
  void Draw(Shader shader)
  {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
      // activate proper texture unit before binding
      glActiveTexture(GL_TEXTURE0 + i);
      // retrieve texture number (the N in the diffuse_textureN)
      string number;
      string name = textures[i].type;
      if (name == "texture_diffuse") {
        number = std::to_string(diffuseNr++);
      } else if (name == "texture_specular") {
        number = std::to_string(specularNr++);
      }
      shader.setFloat(("material." + name + number).c_str(), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  // Render data
  unsigned int VAO, VBO, EBO;
  // Functions
  void SetupMesh()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
  }
};