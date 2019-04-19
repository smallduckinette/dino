#ifndef __ADH_SHADER_H__
#define __ADH_SHADER_H__

#include <istream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>


namespace adh
{
  class Shader
  {
  public:
    Shader(std::istream & vertex,
           std::istream & fragment,
           const std::vector<std::string> & defines = {});
    
    ~Shader();
    
    void use();
    
    void setMatrix(const std::string & name, const glm::mat4 & mat);
    void setVector(const std::string & name, const glm::vec3 & vec);
    void setVector(const std::string & name, const glm::vec4 & vec);
    void setInteger(const std::string & name, int value);
    
  private:
    void compile(GLuint id,
                 std::istream & str,
                 const std::vector<std::string> & defines) const;
    
    GLuint _programId;
  };
}

#endif
