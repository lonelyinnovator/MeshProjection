#include "shader.h"

Shader::Shader(const char *vertex_shader_source_file_path, const char *fragment_shader_source_file_path) {
  // get shader source code from the file path
  std::string vertex_shader_string;
  std::string fragment_shader_string;

  // source code file stream
  std::ifstream vertex_shader_file;
  std::ifstream fragment_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open files
    vertex_shader_file.open(vertex_shader_source_file_path);
    fragment_shader_file.open(fragment_shader_source_file_path);
    // string stream
    std::stringstream vertexShaderStream, fragmentShaderStream;
    // read file's buffer contents into streams
    vertexShaderStream << vertex_shader_file.rdbuf();
    fragmentShaderStream << fragment_shader_file.rdbuf();
    // close files
    vertex_shader_file.close();
    fragment_shader_file.close();
    // convert stream to string
    vertex_shader_string = vertexShaderStream.str();
    fragment_shader_string = fragmentShaderStream.str();
  }
  catch (const std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
  }

  const char *vertex_shader_source = vertex_shader_string.c_str();
  const char *fragment_shader_source = fragment_shader_string.c_str();

  // shader
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  CheckShaderErrors(vertex_shader, "VERTEX");
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  CheckShaderErrors(fragment_shader, "FRAGMENT");

  // shader program
  program_id_ = glCreateProgram();
  glAttachShader(program_id_, vertex_shader);
  glAttachShader(program_id_, fragment_shader);
  glLinkProgram(program_id_);
  CheckShaderErrors(program_id_, "PROGRAM");

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

}


Shader::~Shader() {

}

void Shader::Use() {
  glUseProgram(program_id_);
}

void Shader::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(program_id_, name.c_str()), (int) value);
}

void Shader::SetInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}


void Shader::SetMat3(const std::string &name, glm::mat3 &value) const {
  glUniformMatrix3fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string &name, glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string &name, glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(program_id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(program_id_, name.c_str()), x, y, z);
}


unsigned int Shader::GetProgramId() const {
  return program_id_;
}


void Shader::CheckShaderErrors(unsigned int shader, std::string type) {
  int success;
  char info[1024];
  if (type == "PROGRAM") {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, info);
      std::cout << "Shader program linking failed of type " << type << std::endl << info << std::endl;
    }
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, info);
      std::cout << "Shader compile failed of type " << type << std::endl << info << std::endl;
    }
  }
}

