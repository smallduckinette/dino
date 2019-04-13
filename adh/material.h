#ifndef __ADH_MATERIAL_H__
#define __ADH_MATERIAL_H__

#include "node.h"

namespace adh
{
  class Texture;
  class Shader;
  
  class Material : public Node
  {
  public:
    Material(const std::string & name,
             const std::shared_ptr<Shader> & shader,
             const std::shared_ptr<Texture> & texture);
    
    void draw(Context & context) const override;
    
  private:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Texture> _texture;
  };
}

#endif
