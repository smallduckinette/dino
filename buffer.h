#ifndef __DINO2_BUFFER_H__
#define __DINO2_BUFFER_H__

#include <memory>
#include <json/json.h>
#include <GL/glew.h>

namespace gltf
{
  class Buffer
  {
  public:
    Buffer(const Json::Value & doc);
    
    const char * getData() const;
    size_t getSize() const;
    
  private:
    std::string _data;
  };

  class BufferView
  {
  public:
    BufferView(const Json::Value & doc,
               const std::vector<std::shared_ptr<Buffer> > & buffers);
    
    const char * getData() const;
    size_t getSize() const;
    
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
    
    const char * getData() const;
    size_t getCount() const;
    GLenum getComponentType() const;
    size_t getComponentSize() const;
    size_t getSize() const;
    
    template<typename TYPE>
    void print(std::ostream & str) const
    {
      const TYPE * data = reinterpret_cast<const TYPE *>(getData());
      
      str << "[ ";
      for(size_t index = 0; index < getCount(); ++index)
      {
        str << data[index] << " ";
      }
      str << "]";
    }
    
  private:
    std::shared_ptr<BufferView> _bufferView;
    GLenum _componentType;
    size_t _count;
  };

  std::ostream & operator<<(std::ostream & str, const Accessor & accessor);
  
  std::vector<std::shared_ptr<Accessor> > loadAccessors(const Json::Value & doc);
}

#endif
