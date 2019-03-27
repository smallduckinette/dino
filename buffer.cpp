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

gltf::Accessor::Accessor(const Json::Value & doc,
                         const std::vector<std::shared_ptr<BufferView> > & bufferViews)
{
  size_t viewIndex = doc.get("bufferView", -1).asUInt();
  if(viewIndex >= bufferViews.size())
    throw std::runtime_error("Accessor buffer view reference out of bounds");
  _bufferView = bufferViews.at(viewIndex);
  _count = doc.get("count", 0).asUInt();
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
