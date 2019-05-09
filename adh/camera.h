#ifndef __ADH_CAMERA_H__
#define __ADH_CAMERA_H__

#include "node.h"

namespace adh
{
  class Camera : public Node
  {
  public:
    Camera(float fov,
           float ratio,
           float minDistance,
           float maxDistance,
           const std::string & name = "camera");
    
    void draw(Context & context) const override;
    
    void updateView(float delta_heading, float delta_pitch);
    
  private:
    glm::mat4 _projection;
    float _heading;
    float _pitch;
  };
}

#endif
