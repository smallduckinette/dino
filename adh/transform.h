#ifndef __ADH_TRANSFORM_H__
#define __ADH_TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "node.h"

namespace adh
{
  class Transform : public Node
  {
  public:
    Transform();
    
    void draw(Context & context) const override;
    
    void setMatrix(const glm::mat4 & matrix);
    
    void setTranslate(const glm::vec3 & translate);
    void setRotate(const glm::quat & rotate);
    void setScale(const glm::vec3 & scale);
    
  private:
    void recomputeMatrix();
    
    glm::mat4 _matrix;
    glm::mat4 _translate;
    glm::mat4 _rotate;
    glm::mat4 _scale;
  };
}

#endif
