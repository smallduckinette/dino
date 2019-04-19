#ifndef __ADH_MATERIAL_H__
#define __ADH_MATERIAL_H__

#include "node.h"

namespace adh
{
  class Texture;
  class Shader;
  class Color;
  
  class Material : public Node
  {
  public:
    Material(const std::string & name,
             const std::shared_ptr<Shader> & shader,
             const std::vector<std::shared_ptr<Color> > & colors);
    
    void draw(Context & context) const override;
    
  private:
    std::shared_ptr<Shader> _shader;
    std::vector<std::shared_ptr<Color> > _colors;
  };
}

#endif
