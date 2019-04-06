#include "nodecache.h"

#include <numeric>
#include <boost/log/trivial.hpp>

#include "adh/primitive.h"

#include "accessor.h"
#include "bufferview.h"
#include "buffer.h"


gltf::NodeCache::NodeCache(std::istream & gltfFile)
{
  gltfFile >> _document;
}

std::shared_ptr<adh::Node> gltf::NodeCache::getMesh(size_t index)
{
  auto it = _meshCache.find(index);
  if(it != _meshCache.end())
  {
    return it->second;
  }
  else
  {
    auto && meshDoc = _document["meshes"][Json::ArrayIndex(index)];
    
    auto mesh = std::make_shared<adh::Node>();
    for(auto && primitiveDoc : meshDoc["primitives"])
    {
      mesh->addChild(getPrimitive(primitiveDoc));
    }
    
    return _meshCache.insert({index, mesh}).first->second;
  }
}

std::shared_ptr<adh::Primitive> gltf::NodeCache::getPrimitive(const Json::Value & primitiveDoc)
{
  std::vector<std::shared_ptr<Accessor> > selectedAccessors;
  selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("POSITION", "").asUInt()));
  selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("NORMAL", "").asUInt()));
  selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("TEXCOORD_0", "").asUInt()));
  
  for(auto && accessor : selectedAccessors)
  {
    BOOST_LOG_TRIVIAL(debug) << *accessor;
  }
  
  size_t expectedCount = selectedAccessors.at(0)->getCount();
  for(auto && accessor : selectedAccessors)
  {
    if(accessor->getCount() != expectedCount)
      throw std::runtime_error("Inconsistent accessor count");
  }
    
  size_t totalSize = std::accumulate(selectedAccessors.begin(),
                                     selectedAccessors.end(),
                                     0,
                                     [](auto && acc, auto && accessor)
                                     {
                                       return acc + accessor->getSize();
                                     });
  size_t totalStride = std::accumulate(selectedAccessors.begin(),
                                       selectedAccessors.end(),
                                       0,
                                       [](auto && acc, auto && accessor)
                                       {
                                         return acc +
                                         accessor->getTypeSize() *
                                         accessor->getComponentSize();
                                       });
    
  std::vector<char> data;
  data.reserve(totalSize);
    
  // Stride the data
  for(size_t index = 0; index < expectedCount; ++index)
  {
    for(auto && accessor : selectedAccessors)
    {
      size_t stride = accessor->getTypeSize() * accessor->getComponentSize();
      std::copy(accessor->getData() + index * stride,
                accessor->getData() + (index + 1) * stride,
                std::back_inserter(data));
    }
  }
  
  auto primitive = std::make_shared<adh::Primitive>();
  primitive->bind();
  
  BOOST_LOG_TRIVIAL(debug) << "Copying " << data.size() << " bytes of data out of expected " << totalSize;
  primitive->setVertexData(&data[0], data.size());
    
  size_t index = 0;
  size_t offset = 0;
  for(auto && accessor : selectedAccessors)
  {
    primitive->describeVertexData(index,
                                  accessor->getTypeSize(),
                                  accessor->getComponentType(),
                                  totalStride,
                                  offset);
    ++index;
    offset += accessor->getTypeSize() * accessor->getComponentSize();
  }
    
  BOOST_LOG_TRIVIAL(debug) << "Binding elements...";
  auto indicesAccessor = getAccessor(primitiveDoc.get("indices", "").asUInt());
  primitive->setIndexData(indicesAccessor->getData(),
                          indicesAccessor->getSize());
  primitive->describeIndexData(indicesAccessor->getCount(),
                               indicesAccessor->getComponentType());
  return primitive;
}

std::shared_ptr<gltf::Accessor> gltf::NodeCache::getAccessor(size_t index)
{
  auto it = _accessorCache.find(index);
  if(it != _accessorCache.end())
  {
    return it->second;
  }
  else
  {
    auto && accessorDoc = _document["accessors"][Json::ArrayIndex(index)];
    
    return _accessorCache.insert({index, std::make_shared<Accessor>(accessorDoc, *this)}).first->second;
  }  
}

std::shared_ptr<gltf::BufferView> gltf::NodeCache::getBufferView(size_t index)
{
  auto it = _bufferViewCache.find(index);
  if(it != _bufferViewCache.end())
  {
    return it->second;
  }
  else
  {
    auto && bufferViewDoc = _document["bufferViews"][Json::ArrayIndex(index)];
    
    return _bufferViewCache.insert({index, std::make_shared<BufferView>(bufferViewDoc, *this)}).first->second;
  }    
}

std::shared_ptr<gltf::Buffer> gltf::NodeCache::getBuffer(size_t index)
{
  auto it = _bufferCache.find(index);
  if(it != _bufferCache.end())
  {
    return it->second;
  }
  else
  {
    auto && bufferDoc = _document["buffers"][Json::ArrayIndex(index)];
    
    return _bufferCache.insert({index, std::make_shared<Buffer>(bufferDoc)}).first->second;
  }    
}
