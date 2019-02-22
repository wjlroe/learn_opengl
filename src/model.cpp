unsigned int
TextureFromFile(const char* Path, const std::string& Directory);

class Model
{
public:
  // Functions
  Model(std::string const& Path) { LoadModel(Path); }
  void Draw(Shader shader)
  {
    for (unsigned int i = 0; i < meshes.size(); i++) {
      meshes[i].Draw(shader);
    }
  }

private:
  // Model data
  vector<Mesh> meshes;
  std::string directory;
  vector<Texture> textures_loaded;
  // Functions
  void LoadModel(std::string const& Path)
  {
    Assimp::Importer import;
    const aiScene* Scene =
      import.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !Scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
      return;
    }
    directory = Path.substr(0, Path.find_last_of('/'));

    ProcessNode(Scene->mRootNode, Scene);
  }

  void ProcessNode(aiNode* Node, const aiScene* Scene)
  {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
      aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
      meshes.push_back(ProcessMesh(Mesh, Scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < Node->mNumChildren; i++) {
      ProcessNode(Node->mChildren[i], Scene);
    }
  }

  Mesh ProcessMesh(aiMesh* InputMesh, const aiScene* Scene)
  {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < InputMesh->mNumVertices; i++) {
      Vertex vertex;
      // process vertex positions, normals and texture coordinates
      glm::vec3 vector;
      vector.x = InputMesh->mVertices[i].x;
      vector.y = InputMesh->mVertices[i].y;
      vector.z = InputMesh->mVertices[i].z;
      vertex.Position = vector;
      vector.x = InputMesh->mNormals[i].x;
      vector.y = InputMesh->mNormals[i].y;
      vector.z = InputMesh->mNormals[i].z;
      vertex.Normal = vector;
      if (InputMesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x = InputMesh->mTextureCoords[0][i].x;
        vec.y = InputMesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
      } else {
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }
      vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < InputMesh->mNumFaces; i++) {
      aiFace Face = InputMesh->mFaces[i];
      for (unsigned int j = 0; j < Face.mNumIndices; j++) {
        indices.push_back(Face.mIndices[j]);
      }
    }

    // processs material
    if (InputMesh->mMaterialIndex >= 0) {
      aiMaterial* Material = Scene->mMaterials[InputMesh->mMaterialIndex];
      vector<Texture> diffuseMaps = LoadMaterialTextures(
        Material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      vector<Texture> specularMaps = LoadMaterialTextures(
        Material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
  }

  vector<Texture> LoadMaterialTextures(aiMaterial* Material,
                                       aiTextureType Type,
                                       std::string TypeName)
  {
    vector<Texture> textures;
    for (unsigned int i = 0; i < Material->GetTextureCount(Type); i++) {
      aiString str;
      Material->GetTexture(Type, i, &str);
      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          skip = true;
          break;
        }
      }
      if (!skip) {
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = TypeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        textures_loaded.push_back(texture);
      }
    }
    return textures;
  }
};

unsigned int
TextureFromFile(const char* Path, const std::string& Directory)
{
  std::string Filename = std::string(Path);
  Filename = Directory + '/' + Filename;

  unsigned int TextureID;
  glGenTextures(1, &TextureID);

  int Width, Height, NumChannels;
  unsigned char* Data =
    stbi_load(Filename.c_str(), &Width, &Height, &NumChannels, 0);
  if (Data) {
    GLenum Format;
    if (NumChannels == 1) {
      Format = GL_RED;
    } else if (NumChannels == 3) {
      Format = GL_RGB;
    } else if (NumChannels == 4) {
      Format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, TextureID);
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(Data);
  } else {
    std::cout << "Texture failed to load at path: " << Path << std::endl;
    stbi_image_free(Data);
  }
  return TextureID;
}
