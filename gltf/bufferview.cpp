#include "bufferview.h"

#include "nodecache.h"
#include "buffer.h"

gltf::OldBufferView::OldBufferView(const Json::Value & doc,
                                   NodeCache & nodeCache)
{
  size_t bufferIndex = doc.get("buffer", -1).asUInt();
  _buffer = nodeCache.getBuffer(bufferIndex);
  
  _length = doc.get("byteLength", 0).asUInt();
  _offset = doc.get("byteOffset", 0).asUInt();
}

const char * gltf::OldBufferView::getData() const
{
  return _buffer->getData() + _offset;
}

size_t gltf::OldBufferView::getSize() const
{
  return _length;
}
