#include "shader.h"

#include <iterator>
#include <iostream>


Shader::Shader(std::istream & vertex,
               std::istream & fragment):
  _programId(glCreateProgram())
{
  GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  
  compile(vertexId, vertex);
  compile(fragmentId, fragment);
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

Shader::~Shader()
{
  glDeleteProgram(_programId);
}

void Shader::use()
{
  glUseProgram(_programId);
}

void Shader::compile(GLuint id, std::istream & str) const
{
  auto source = std::string(std::istreambuf_iterator<char>(str),
                            std::istreambuf_iterator<char>());
  const char * csource = source.c_str();
  glShaderSource(id, 1, &csource, NULL);
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
