#ifndef __ADH_TRANSFORM_H__
#define __ADH_TRANSFORM_H__

#include "node.h"

namespace adh
{
  class Transform : public Node
  {
  public:
    Transform();
    
    void draw(Context & context) const override;
    
    void setMatrix(const glm::mat4 & matrix);
    
  private:
    glm::mat4 _matrix;
  };
}

#endif
