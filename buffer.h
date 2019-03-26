#ifndef __DINO2_BUFFER_H__
#define __DINO2_BUFFER_H__

#include <memory>
#include <json/json.h>

namespace gltf
{
  class Buffer
  {
  public:
    Buffer(const Json::Value & doc);
    
  private:
    std::string _data;
  };

  class BufferView
  {
  public:
    BufferView(const Json::Value & doc,
               const std::vector<std::shared_ptr<Buffer> > & buffers);

  private:
    size_t _length;
    size_t _offset;
    std::shared_ptr<Buffer> _buffer;
  };

  class Accessor
  {
  public:
    Accessor(const Json::Value & doc,
             const std::vector<std::shared_ptr<BufferView> > & bufferViews);
    
  private:
    std::shared_ptr<BufferView> _bufferView;
    size_t _count;
  };
}

#endif
