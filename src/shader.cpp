struct Shader
{
  unsigned int ID;

  Shader(const char* VertexPath, const char* FragmentPath)
  {
    char* vShaderCode = LoadFileContents(VertexPath);
    char* fShaderCode = LoadFileContents(FragmentPath);

    unsigned int Vertex, Fragment;
    int Success;
    char InfoLog[512];

    Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertex, 1, &vShaderCode, NULL);
    glCompileShader(Vertex);
    free(vShaderCode);

    glGetShaderiv(Vertex, GL_COMPILE_STATUS, &Success);
    if (!Success) {
      glGetShaderInfoLog(Vertex, 512, NULL, InfoLog);
      std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << "SHADER SOURCE:\n"
                << vShaderCode << "\nERROR:\n"
                << InfoLog << std::endl;
    }

    Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, &fShaderCode, NULL);
    glCompileShader(Fragment);
    free(fShaderCode);

    glGetShaderiv(Fragment, GL_COMPILE_STATUS, &Success);
    if (!Success) {
      glGetShaderInfoLog(Fragment, 512, NULL, InfoLog);
      std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << "SHADER SOURCE:\n"
                << fShaderCode << "\nERROR:\n"
                << InfoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, Vertex);
    glAttachShader(ID, Fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &Success);
    if (!Success) {
      glGetProgramInfoLog(ID, 512, NULL, InfoLog);
      std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << InfoLog << std::endl;
    }

    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
  }

  void use() { glUseProgram(ID); }

  void setBool(const char* Name, bool Value)
  {
    glUniform1i(glGetUniformLocation(ID, Name), (int)Value);
  }

  void setInt(const char* Name, int Value)
  {
    glUniform1i(glGetUniformLocation(ID, Name), Value);
  }

  void setFloat(const char* Name, float Value)
  {
    glUniform1f(glGetUniformLocation(ID, Name), Value);
  }

  void setVec2(const char* Name, float x, float y)
  {
    glUniform2f(glGetUniformLocation(ID, Name), x, y);
  }

  void setVec3(const char* Name, float x, float y, float z)
  {
    glUniform3f(glGetUniformLocation(ID, Name), x, y, z);
  }

  void setVec3(const char* Name, const glm::vec3& Value)
  {
    glUniform3fv(glGetUniformLocation(ID, Name), 1, &Value[0]);
  }

  void setVec4(const char* Name, float x, float y, float z, float w)
  {
    glUniform4f(glGetUniformLocation(ID, Name), x, y, z, w);
  }

  void setVec4(const char* Name, const glm::vec4& Value) const
  {
    glUniform4fv(glGetUniformLocation(ID, Name), 1, &Value[0]);
  }

  void setMat4(const char* Name, const glm::mat4& Matrix) const
  {
    glUniformMatrix4fv(
      glGetUniformLocation(ID, Name), 1, GL_FALSE, &Matrix[0][0]);
  }
};
