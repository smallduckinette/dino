#ifndef __DINO3_PARSER_CPP__
#define __DINO3_PARSER_CPP__

#include <vector>
#include <memory>

#include "buffer.h"

namespace gltf
{
  class Scene;
  
  std::vector<std::shared_ptr<Scene> > parse(std::istream & str);
}

#endif
