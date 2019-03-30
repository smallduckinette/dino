#include "buffer.h"

#include <stdexcept>

#include "base64.h"
#include <boost/algorithm/string/predicate.hpp>


gltf::Buffer::Buffer(const Json::Value & doc)
{
  size_t length = doc.get("byteLength", 0).asUInt();
  std::string b64data = doc.get("uri", "").asString();
  
  std::string header("data:application/octet-stream;base64,");
  if(!boost::algorithm::starts_with(b64data, header))
    throw std::runtime_error("Unrecognized doc format");
  
  _data = base64_decode(b64data.begin() + header.size(), b64data.end());
  
  if(_data.size() != length)
    throw std::runtime_error("Inconsistent buffer size");
}

const char * gltf::Buffer::getData() const
{
  return _data.data();
}

size_t gltf::Buffer::getSize() const
{
  return _data.size();
}

gltf::BufferView::BufferView(const Json::Value & doc,
                             const std::vector<std::shared_ptr<Buffer> > & buffers)
{
  size_t bufferIndex = doc.get("buffer", -1).asUInt();
  if(bufferIndex >= buffers.size())
    throw std::runtime_error("BufferView buffer reference out of bounds");
  _buffer = buffers.at(bufferIndex);
  
  _length = doc.get("byteLength", 0).asUInt();
  _offset = doc.get("byteOffset", 0).asUInt();
}

const char * gltf::BufferView::getData() const
{
  return _buffer->getData() + _offset;
}

size_t gltf::BufferView::getSize() const
{
  return _length;
}

gltf::Accessor::Accessor(const Json::Value & doc,
                         const std::vector<std::shared_ptr<BufferView> > & bufferViews)
{
  size_t viewIndex = doc.get("bufferView", -1).asUInt();
  if(viewIndex >= bufferViews.size())
    throw std::runtime_error("Accessor buffer view reference out of bounds");
  _bufferView = bufferViews.at(viewIndex);
  
  switch(doc.get("componentType", 0).asUInt())
  {
  case 5120:
    _componentType = GL_BYTE;
    break;
  case 5121:
    _componentType = GL_UNSIGNED_BYTE;
    break;
  case 5122:
    _componentType = GL_SHORT;
    break;
  case 5123:
    _componentType = GL_UNSIGNED_SHORT;
    break;
  case 5125:
    _componentType = GL_UNSIGNED_INT;
    break;
  case 5126:
    _componentType = GL_FLOAT;
    break;
  default:
    throw std::runtime_error("Unknown component type " + doc.get("componentType", 0).asString());
  }
  
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

std::vector<std::shared_ptr<gltf::Accessor> > gltf::loadAccessors(const Json::Value & doc)
{
  // Load buffers
  std::vector<std::shared_ptr<Buffer> > buffers;
  for(auto && val : doc["buffers"])
  {
    buffers.push_back(std::make_shared<Buffer>(val));
  }

  // Load buffer views
  std::vector<std::shared_ptr<BufferView> > bufferViews;
  for(auto && val : doc["bufferViews"])
  {
    bufferViews.push_back(std::make_shared<BufferView>(val, buffers));
  }

  // Load accessors
  std::vector<std::shared_ptr<Accessor> > accessors;
  for(auto && val : doc["accessors"])
  {
    accessors.push_back(std::make_shared<Accessor>(val, bufferViews));
  }
  
  return accessors;
}
