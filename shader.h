#ifndef __DINO2_SHADER_H__
#define __DINO2_SHADER_H__

#include <istream>

#include <GL/glew.h>
#include <glm/glm.hpp>


class Shader
{
public:
  Shader(std::istream & vertex,
         std::istream & fragment);
  
  ~Shader();
  
  void use();

  void setMatrix(const std::string & name, const glm::mat4 & mat);
  
private:
  void compile(GLuint id, std::istream & str) const;
  
  GLuint _programId;
};

#endif
