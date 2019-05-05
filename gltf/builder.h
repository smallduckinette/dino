#ifndef __GLTF_BUILDER_H__
#define __GLTF_BUILDER_H__

#include <string>
#include <memory>
#include <vector>
#include <filesystem>

namespace adh { class Node; }
namespace adh { class Transform; }
namespace adh { class Primitive; }
namespace adh { class Shader; }

namespace gltf
{
  class Asset;
  class Accessor;
  
  class Builder
  {
  public:
    Builder(const std::string & shaderPath,
            const std::string & gltfFile);
    
    std::unique_ptr<adh::Node> build() const;
    
  private:
    std::unique_ptr<adh::Node> buildNode(size_t nodeIndex) const;
    std::unique_ptr<adh::Node> buildMesh(size_t meshIndex) const;
    
    void setIndicesBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                          const gltf::Accessor & accessor) const;
    size_t setDataBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                         const gltf::Accessor & accessor) const;
    
    std::shared_ptr<adh::Shader> getShader(const std::vector<std::string> & defines) const;
    
    std::filesystem::path _shaderPath;
    std::shared_ptr<Asset> _asset;
  };
}

#endif
