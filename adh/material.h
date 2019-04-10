#ifndef __ADH_MATERIAL_H__
#define __ADH_MATERIAL_H__

#include "node.h"

namespace adh
{
  class Texture;
  
  class Material : public Node
  {
  public:
    Material(const std::string & name,
             const std::shared_ptr<Texture> & texture);
    
    void draw() const override;
    
  private:
    std::shared_ptr<Texture> _texture;
  };
}

#endif
