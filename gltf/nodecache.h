#ifndef __GLTF_NODECACHE_H__
#define __GLTF_NODECACHE_H__

#include <memory>
#include <filesystem>
#include <json/json.h>

namespace adh { class Node; }
namespace adh { class Primitive; }
namespace adh { class Texture; }
namespace adh { class Material; }

namespace gltf
{
  class Accessor;
  class BufferView;
  class Buffer;
  
  class NodeCache
  {
  public:
    NodeCache(const std::string & gltfFile);
    NodeCache(const NodeCache &) = delete;
    NodeCache & operator=(const NodeCache &) = delete;
    
    std::shared_ptr<adh::Node> getMesh(size_t index);
    
    std::shared_ptr<Accessor> getAccessor(size_t index);
    std::shared_ptr<BufferView> getBufferView(size_t index);
    std::shared_ptr<Buffer> getBuffer(size_t index);
    std::shared_ptr<adh::Texture> getTexture(size_t index);
    std::shared_ptr<adh::Material> getMaterial(size_t index);
    
  private:
    std::shared_ptr<adh::Node> getPrimitive(const Json::Value & primitiveDoc);
    
    std::map<size_t, std::shared_ptr<adh::Node> > _meshCache;
    
    std::map<size_t, std::shared_ptr<gltf::Accessor> > _accessorCache;
    std::map<size_t, std::shared_ptr<gltf::BufferView> > _bufferViewCache;
    std::map<size_t, std::shared_ptr<gltf::Buffer> > _bufferCache;
    std::map<size_t, std::shared_ptr<adh::Texture> > _textureCache;
    std::map<size_t, std::shared_ptr<adh::Material> > _materialCache;
    
    std::filesystem::path _modelPath;
    Json::Value _document;
  };
}

#endif
