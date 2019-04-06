#ifndef __GLTF_ACCESSOR_H__
#define __GLTF_ACCESSOR_H__

#include <memory>
#include <json/json.h>
#include <GL/glew.h>


namespace gltf
{
  class NodeCache;
  class BufferView;
  
  class Accessor
  {
  public:
    Accessor(const Json::Value & doc,
             NodeCache & nodeCache);
    
    const char * getData() const;
    size_t getCount() const;
    GLenum getComponentType() const;
    size_t getComponentSize() const;
    size_t getTypeSize() const;
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
    size_t _typeSize;
  };

  std::ostream & operator<<(std::ostream & str, const Accessor & accessor);
}

#endif
