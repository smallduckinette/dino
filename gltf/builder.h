#ifndef __GLTF_BUILDER_H__
#define __GLTF_BUILDER_H__

#include <string>
#include <memory>

namespace adh { class Node; }
namespace adh { class Transform; }

namespace gltf
{
  class Asset;
  
  class Builder
  {
  public:
    Builder(const std::string & gltfFile);
    
    std::unique_ptr<adh::Node> build() const;
    
  private:
    std::unique_ptr<adh::Node> buildNode(size_t nodeIndex) const;
    std::unique_ptr<adh::Node> buildMesh(size_t meshIndex) const;
    
    std::shared_ptr<Asset> _asset;
  };
}

#endif
