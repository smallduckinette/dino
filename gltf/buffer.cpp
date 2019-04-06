#include "buffer.h"

#include <fstream>
#include <iterator>
#include <boost/algorithm/string/predicate.hpp>

#include "base64.h"


gltf::Buffer::Buffer(const Json::Value & doc)
{
  size_t length = doc.get("byteLength", 0).asUInt();
  std::string uri = doc.get("uri", "").asString();
  
  std::string inlineHeader("data:application/octet-stream;base64,");
  if(boost::algorithm::starts_with(uri, inlineHeader))
  {
    _data = base64_decode(uri.begin() + inlineHeader.size(), uri.end());
  }
  else
  {
    std::ifstream file(uri);
    std::copy(std::istreambuf_iterator<char>(file),
              std::istreambuf_iterator<char>(),
              std::back_inserter(_data));
  }
  
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
