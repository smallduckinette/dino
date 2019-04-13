#ifndef __ADH_SHADER_H__
#define __ADH_SHADER_H__

#include <istream>

#include <GL/glew.h>
#include <glm/glm.hpp>


namespace adh
{
  class Shader
  {
  public:
    Shader(std::istream & vertex,
           std::istream & fragment);
    
    ~Shader();
    
    void use();
    
    void setMatrix(const std::string & name, const glm::mat4 & mat);
    void setInteger(const std::string & name, int value);
    
  private:
    void compile(GLuint id, std::istream & str) const;
    
    GLuint _programId;
  };
}

#endif
