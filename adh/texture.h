#ifndef __ADH_TEXTURE_H__
#define __ADH_TEXTURE_H__

#include <string>
#include <GL/glew.h>

namespace adh
{
  class Texture
  {
  public:
    Texture(const std::string & name,
            const std::string & filename);
    ~Texture();
    
    void bind(GLenum texture);
    std::string getName() const;
    
  private:
    std::string _name;
    GLuint _textureId;
  };
}

#endif
