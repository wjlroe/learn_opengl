char* LoadFileContents(const char* Path)
{
  std::string Contents;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file.open(Path);
    std::stringstream fileStream;

    fileStream << file.rdbuf();

    file.close();

    Contents = fileStream.str();
  } catch (const std::ifstream::failure& e) {
    std::cerr << "ERROR::FILE_NOT_SUCCESSFULLY_READ : " << Path
      << std::endl;
    throw;
  }
  return strdup(Contents.c_str());
}
