#ifndef __ADH_PLAINCOLOR_H__
#define __ADH_PLAINCOLOR_H__

#include <string>
#include <glm/glm.hpp>

#include "color.h"

namespace adh
{
  class PlainColor : public Color
  {
  public:
    PlainColor(const std::string & reference,
               const glm::vec4 & color);

    void bind(Shader & shader) override;
    
  private:
    std::string _reference;
    glm::vec4 _color;
  };
}

#endif
