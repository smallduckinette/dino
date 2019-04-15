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
             const std::shared_ptr<Texture> & diffuseTexture,
             const std::shared_ptr<Texture> & normalTexture,
             const std::shared_ptr<Texture> & metalRoughnessTexture);
    
    void draw(Context & context) const override;
    
  private:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Texture> _diffuseTexture;
    std::shared_ptr<Texture> _normalTexture;
    std::shared_ptr<Texture> _metalRoughnessTexture;
  };
}

#endif
