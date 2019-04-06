#include "accessor.h"

#include "nodecache.h"
#include "bufferview.h"


gltf::Accessor::Accessor(const Json::Value & doc,
                         NodeCache & nodeCache)
{
  size_t viewIndex = doc.get("bufferView", -1).asUInt();
  _bufferView = nodeCache.getBufferView(viewIndex);
  _componentType = doc.get("componentType", 0).asUInt();
  
  std::string type = doc.get("type", "").asString();
  if(type == "SCALAR")
    _typeSize = 1;
  else if(type == "VEC2")
    _typeSize = 2;
  else if(type == "VEC3")
    _typeSize = 3;
  else
    throw std::runtime_error("Unknown type " + type);
  
  _count = doc.get("count", 0).asUInt();
}


const char * gltf::Accessor::getData() const
{
  return _bufferView->getData();
}

size_t gltf::Accessor::getCount() const
{
  return _count;
}

GLenum gltf::Accessor::getComponentType() const
{
  return _componentType;
}

size_t gltf::Accessor::getComponentSize() const
{
  switch(_componentType)
  {
  case GL_BYTE:
  case GL_UNSIGNED_BYTE:
    return 1;
  case GL_SHORT:
  case GL_UNSIGNED_SHORT:
    return 2;
  case GL_UNSIGNED_INT:
  case GL_FLOAT:
    return 4;
  default:
    throw std::runtime_error("Invalid component type" + std::to_string(_componentType));
  }
}

size_t gltf::Accessor::getTypeSize() const
{
  return _typeSize;
}

size_t gltf::Accessor::getSize() const
{
  return getCount() * getComponentSize() * getTypeSize();
}

std::ostream & gltf::operator<<(std::ostream & str, const Accessor & accessor)
{
  switch(accessor.getComponentType())
  {
  case GL_BYTE:
    accessor.print<char>(str);
    break;
  case GL_UNSIGNED_BYTE:
    accessor.print<unsigned char>(str);
    break;    
  case GL_SHORT:
    accessor.print<short int>(str);
    break;    
  case GL_UNSIGNED_SHORT:
    accessor.print<unsigned short int>(str);
    break;        
  case GL_UNSIGNED_INT:
    accessor.print<unsigned int>(str);
    break;        
  case GL_FLOAT:
    accessor.print<float>(str);
    break;        
  default:
    throw std::runtime_error("Invalid component type" + std::to_string(accessor.getComponentType()));
  }  
  
  return str;
}
