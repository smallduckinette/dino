#ifndef __GLTF_BUILDER_H__
#define __GLTF_BUILDER_H__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <filesystem>
#include <glm/glm.hpp>

namespace adh { class Node; }
namespace adh { class Transform; }
namespace adh { class Primitive; }
namespace adh { class Shader; }
namespace adh { class Texture; }
namespace adh { class Clock; }
namespace adh { class Animation; }
namespace adh { template<typename T> class Interpolator; }

namespace gltf
{
  class Asset;
  class Accessor;
  class AnimationSampler;
  
  class Builder
  {
  public:
    Builder(const std::shared_ptr<adh::Clock> & clock,
            const std::string & shaderPath,
            const std::string & gltfFile);
    
    std::unique_ptr<adh::Node> build(std::vector<std::unique_ptr<adh::Animation> > & animations) const;
    
  private:
    std::shared_ptr<adh::Node> buildNode(size_t nodeIndex, std::map<size_t, std::shared_ptr<adh::Transform> > & animationNodes) const;
    std::unique_ptr<adh::Node> buildMesh(size_t meshIndex) const;
    std::unique_ptr<adh::Texture> buildTexture(const std::string & name,
                                               size_t textureIndex) const;
    template<typename T>
    std::unique_ptr<adh::Interpolator<T> > buildInterpolator(const gltf::AnimationSampler & sampler) const;
    
    void setIndicesBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                          const gltf::Accessor & accessor) const;
    size_t setDataBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                         const gltf::Accessor & accessor) const;
    
    std::shared_ptr<adh::Shader> getShader(const std::vector<std::string> & defines) const;
    
    std::shared_ptr<adh::Clock> _clock;
    std::filesystem::path _shaderPath;
    std::shared_ptr<Asset> _asset;
    std::filesystem::path _modelPath;
  };
}

#endif
