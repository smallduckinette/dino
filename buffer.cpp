#include "buffer.h"

#include <stdexcept>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/algorithm/string/predicate.hpp>

gltf::Buffer::Buffer(const Json::Value & buffer)
{
  size_t length = buffer.get("byteLength", 0).asUInt();
  std::string b64data = buffer.get("uri", "").asString();
  
  std::string header("data:application/octet-stream;base64,");
  if(!boost::algorithm::starts_with(b64data, header))
    throw std::runtime_error("Unrecognized buffer format");
  
  typedef boost::archive::iterators::transform_width<
    boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6 > B64Decoder;
  
  _data.assign(B64Decoder(b64data.begin() + header.size()), B64Decoder(b64data.end()));
  
  if(_data.size() != length)
    throw std::runtime_error("Inconsistent buffer size");
}
