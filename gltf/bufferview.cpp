#include "bufferview.h"

#include "nodecache.h"
#include "buffer.h"

gltf::BufferView::BufferView(const Json::Value & doc,
                             NodeCache & nodeCache)
{
  size_t bufferIndex = doc.get("buffer", -1).asUInt();
  _buffer = nodeCache.getBuffer(bufferIndex);
  
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
