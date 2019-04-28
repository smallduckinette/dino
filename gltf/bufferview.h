#ifndef __GLTF_BUFFERVIEW_H__
#define __GLTF_BUFFERVIEW_H__

#include <memory>
#include <json/json.h>

namespace gltf
{
  class NodeCache;
  class OldBuffer;
  
  class OldBufferView
  {
  public:
    OldBufferView(const Json::Value & doc,
                  NodeCache & nodeCache);
      
    const char * getData() const;
    size_t getSize() const;
    
  private:
    size_t _length;
    size_t _offset;
    std::shared_ptr<OldBuffer> _buffer;
  };  
}

#endif
