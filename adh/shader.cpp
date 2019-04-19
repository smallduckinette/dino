#include "shader.h"

#include <iterator>
#include <iostream>
#include <numeric>


adh::Shader::Shader(std::istream & vertex,
                    std::istream & fragment,
                    const std::vector<std::string> & defines):
  _programId(glCreateProgram())
{
  GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  
  compile(vertexId, vertex, defines);
  compile(fragmentId, fragment, defines);
  glAttachShader(_programId, vertexId);
  glAttachShader(_programId, fragmentId);
  glLinkProgram(_programId);
  
  GLint success;
  GLchar infoLog[1024];
  glGetProgramiv(_programId, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(_programId, 1024, NULL, infoLog);
    std::cout << "Error linking shader : " << infoLog << std::endl;
  }  
  
  glDeleteShader(vertexId);
  glDeleteShader(fragmentId);
}

adh::Shader::~Shader()
{
  glDeleteProgram(_programId);
}

void adh::Shader::use()
{
  glUseProgram(_programId);
}

void adh::Shader::setMatrix(const std::string & name, const glm::mat4 & mat)
{
  glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void adh::Shader::setVector(const std::string & name, const glm::vec3 & vec)
{
  glUniform3fv(glGetUniformLocation(_programId, name.c_str()), 1, &vec[0]);
}

void adh::Shader::setVector(const std::string & name, const glm::vec4 & vec)
{
  glUniform4fv(glGetUniformLocation(_programId, name.c_str()), 1, &vec[0]);
}

void adh::Shader::setInteger(const std::string & name, int value)
{
  glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
}

void adh::Shader::compile(GLuint id,
                          std::istream & str,
                          const std::vector<std::string> & defines) const
{
  std::string version = "#version 330 core\n";
  auto defs = std::accumulate(defines.begin(),
                              defines.end(),
                              std::string(),
                              [](auto && acc, auto && d) { return acc + "#define " + d + "\n"; });
  
  auto source = std::string(std::istreambuf_iterator<char>(str),
                            std::istreambuf_iterator<char>());
  const char * sources[] = {version.c_str(), defs.c_str(), source.c_str()};
  glShaderSource(id, 3, sources, NULL);
  glCompileShader(id);
  
  GLint success;
  GLchar infoLog[1024];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(id, 1024, NULL, infoLog);
    std::cout << "Error compiling shader : " << infoLog << std::endl;
  }
}
