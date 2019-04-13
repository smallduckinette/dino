#ifndef __ADH_CONTEXT_H__
#define __ADH_CONTEXT_H__

#include <glm/glm.hpp>

namespace adh
{
  class Context
  {
  public:
    Context();
    
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
  };
}

#endif
