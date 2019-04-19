#ifndef __ADH_TEXTURECOLOR_H__
#define __ADH_TEXTURECOLOR_H__

#include <memory>
#include <string>
#include <GL/glew.h>

#include "color.h"

namespace adh
{
  class Texture;
  
  class TextureColor : public Color
  {
  public:
    TextureColor(const std::shared_ptr<Texture> & texture,
                 GLenum textureType,
                 const std::string & textureReference);
    
    void bind(Shader & shader) override;
    
  private:
    std::shared_ptr<Texture> _texture;
    GLenum _textureType;
    std::string _textureReference;
  };
}

#endif
