#ifndef __GLTF_NODECACHE_H__
#define __GLTF_NODECACHE_H__

#include <memory>
#include <filesystem>
#include <json/json.h>

namespace adh { class Node; }
namespace adh { class Primitive; }
namespace adh { class Texture; }
namespace adh { class Material; }
namespace adh { class Shader; }

namespace gltf
{
  class OldAccessor;
  class OldBufferView;
  class OldBuffer;
  
  class NodeCache
  {
  public:
    NodeCache(const std::string & shaderDir,
              const std::string & gltfFile);
    NodeCache(const NodeCache &) = delete;
    NodeCache & operator=(const NodeCache &) = delete;
    
    std::shared_ptr<adh::Node> getScene();
    
    std::shared_ptr<adh::Node> getMesh(size_t index);
    std::shared_ptr<OldAccessor> getAccessor(size_t index);
    std::shared_ptr<OldBufferView> getBufferView(size_t index);
    std::shared_ptr<OldBuffer> getBuffer(size_t index);
    std::shared_ptr<adh::Texture> getTexture(size_t index);
    std::shared_ptr<adh::Material> getMaterial(size_t index);
    
  private:
    std::shared_ptr<adh::Node> getPrimitive(const Json::Value & primitiveDoc);
    std::shared_ptr<adh::Shader> getShader(const std::string & shaderName,
                                           const std::vector<std::string> & defines);
    
    std::map<size_t, std::shared_ptr<adh::Node> > _meshCache;
    
    std::map<size_t, std::shared_ptr<gltf::OldAccessor> > _accessorCache;
    std::map<size_t, std::shared_ptr<gltf::OldBufferView> > _bufferViewCache;
    std::map<size_t, std::shared_ptr<gltf::OldBuffer> > _bufferCache;
    std::map<size_t, std::shared_ptr<adh::Texture> > _textureCache;
    std::map<size_t, std::shared_ptr<adh::Material> > _materialCache;
    std::map<std::pair<std::string, std::vector<std::string> >, std::shared_ptr<adh::Shader> > _shaderCache;
    
    std::filesystem::path _shaderPath;
    std::filesystem::path _modelPath;
    Json::Value _document;
  };
}

#endif
