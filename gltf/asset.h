#ifndef __GLTF_ASSET_H__
#define __GLTF_ASSET_H__

#include <string>
#include <memory>
#include <filesystem>
#include <optional>
#include <vector>
#include <json/json.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>


namespace gltf
{
  class Scene
  {
  public:
    Scene(const Json::Value & sceneDocument);
    Scene(const std::optional<std::string> & name,
          const std::vector<size_t> & nodes);
    
    std::optional<std::string> _name;
    std::vector<size_t> _nodes;
    
    bool operator==(const Scene & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const Scene & scene);
  
  class Node
  {
  public:
    Node(const Json::Value & nodeDocument);
    Node(const std::optional<size_t> & camera,
         const std::vector<size_t> & children,
         const std::optional<size_t> & skin,
         const std::optional<glm::mat4> & matrix,
         const std::optional<size_t> & mesh,
         const std::optional<glm::quat> & rotation,
         const std::optional<glm::vec3> & scale,
         const std::optional<glm::vec3> & translation,
         const std::vector<size_t> & weights,
         const std::optional<std::string> & name);
    
    std::optional<size_t> _camera;
    std::vector<size_t> _children;
    std::optional<size_t> _skin;
    std::optional<glm::mat4> _matrix;
    std::optional<size_t> _mesh;
    std::optional<glm::quat> _rotation;
    std::optional<glm::vec3> _scale;
    std::optional<glm::vec3> _translation;
    std::vector<size_t> _weights;
    std::optional<std::string> _name;

    bool operator==(const Node & other) const;
  };

  std::ostream & operator<<(std::ostream & str, const Node & node);

  class Primitive
  {
  public:
    Primitive(const Json::Value & primitiveDocument);
    Primitive(const std::map<std::string, size_t> & attributes,
              const std::optional<size_t> & indices,
              const std::optional<size_t> & material,
              GLenum mode,
              const std::vector<std::map<std::string, size_t> > & targets);
    
    std::map<std::string, size_t> _attributes;
    std::optional<size_t> _indices;
    std::optional<size_t> _material;
    GLenum _mode;
    std::vector<std::map<std::string, size_t> > _targets;
    
    bool operator==(const Primitive & other) const;
  };

  std::ostream & operator<<(std::ostream & str, const Primitive & primitive);
  
  class Mesh
  {
  public:
    Mesh(const Json::Value & meshDocument);
    Mesh(const std::vector<Primitive> & primitives,
         const std::vector<size_t> & weights,
         const std::optional<std::string> & name);
    
    
    std::vector<Primitive> _primitives;
    std::vector<size_t> _weights;
    std::optional<std::string> _name;

    bool operator==(const Mesh & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const Mesh & mesh);
  
  class Buffer
  {
  public:
    Buffer(const Json::Value & sceneDocument,
           const std::filesystem::path & modelPath);
    
    std::string _data;
    std::optional<std::string> _name;
  };

  class BufferView
  {
  public:
    BufferView(const Json::Value & bufferViewDocument);
    BufferView(size_t buffer,
               size_t byteOffset,
               size_t byteLength,
               size_t byteStride,
               const std::optional<GLenum> & target,
               const std::optional<std::string> & name);

    size_t _buffer;
    size_t _byteOffset;
    size_t _byteLength;
    size_t _byteStride;
    std::optional<GLenum> _target;
    std::optional<std::string> _name;

    bool operator==(const BufferView & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const BufferView & bufferView);
  
  class Sparse
  {
  public:
    Sparse(const Json::Value & sparseDocument);
    Sparse(size_t count,
           const std::map<std::string, size_t> & indices,
           const std::map<std::string, size_t> & values);
           
    size_t _count;
    std::map<std::string, size_t> _indices;
    std::map<std::string, size_t> _values;
    
    bool operator==(const Sparse & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const Sparse & sparse);
  
  class Accessor
  {
  public:
    Accessor(const Json::Value & accessorDocument);
    Accessor(const std::optional<size_t> & bufferView,
             size_t byteOffset,
             GLenum componentType,
             bool normalized,
             size_t count,
             const std::string & type,
             const std::vector<float> & max,
             const std::vector<float> & min,
             const std::optional<Sparse> & sparse,
             const std::optional<std::string> & name);
    
    std::optional<size_t> _bufferView;
    size_t _byteOffset;
    GLenum _componentType;
    bool _normalized;
    size_t _count;
    std::string _type;
    std::vector<float> _max;
    std::vector<float> _min;
    std::optional<Sparse> _sparse;
    std::optional<std::string> _name;
    
    bool operator==(const Accessor & other) const;
    
    GLint getComponentCount() const;
  };
  
  std::ostream & operator<<(std::ostream & str, const Accessor & accessor);
  
  class TextureInfo
  {
  public:
    TextureInfo(const Json::Value & materialDocument);
    TextureInfo(size_t index,
                size_t texCoord);
    
    size_t _index;
    size_t _texCoord;

    bool operator==(const TextureInfo & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const TextureInfo & textureInfo);
  
  class NormalTextureInfo
  {
  public:
    NormalTextureInfo(const Json::Value & materialDocument);
    NormalTextureInfo(size_t index,
                      size_t texCoord,
                      float scale);
    
    size_t _index;
    size_t _texCoord;
    float _scale;
    
    bool operator==(const NormalTextureInfo & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const NormalTextureInfo & textureInfo);
  
  class OcclusionTextureInfo
  {
  public:
    OcclusionTextureInfo(const Json::Value & materialDocument);
    OcclusionTextureInfo(size_t index,
                      size_t texCoord,
                      float scale);
    
    size_t _index;
    size_t _texCoord;
    float _strength;
    
    bool operator==(const OcclusionTextureInfo & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const OcclusionTextureInfo & textureInfo);
  
  class PbrMetallicRoughness
  {
  public:
    PbrMetallicRoughness(const Json::Value & pbrDocument);
    PbrMetallicRoughness(const glm::vec4 & baseColorFactor,
                         const std::optional<TextureInfo> & baseColorTexture,
                         float metallicFactor,
                         float roughnessFactory,
                         const std::optional<TextureInfo> & metallicRoughnessTexture);

    
    glm::vec4 _baseColorFactor;
    std::optional<TextureInfo> _baseColorTexture;
    float _metallicFactor;
    float _roughnessFactor;
    std::optional<TextureInfo> _metallicRoughnessTexture;

    bool operator==(const PbrMetallicRoughness & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const PbrMetallicRoughness & pbr);
  
  class Material
  {
  public:
    Material(const Json::Value & materialDocument);
    Material(const std::optional<std::string> & name,
             const std::optional<PbrMetallicRoughness> & pbrMetallicRoughness,
             const std::optional<NormalTextureInfo> & normalTexture,
             const std::optional<OcclusionTextureInfo> & occlusionTexture,
             const std::optional<TextureInfo> & emissiveTexture,
             const glm::vec3 & emissiveFactor,
             const std::string & alphaMode,
             float alphaCutoff,
             bool doubleSided);
    
    std::optional<std::string> _name;
    std::optional<PbrMetallicRoughness> _pbrMetallicRoughness;
    std::optional<NormalTextureInfo> _normalTexture;
    std::optional<OcclusionTextureInfo> _occlusionTexture;
    std::optional<TextureInfo> _emissiveTexture;
    glm::vec3 _emissiveFactor;
    std::string _alphaMode;
    float _alphaCutoff;
    bool _doubleSided;

    bool operator==(const Material & other) const;
  };

  std::ostream & operator<<(std::ostream & str, const Material & material);

  class Texture
  {
  public:
    Texture(const Json::Value & textureDocument);
    Texture(const std::optional<size_t> & sampler,
            const std::optional<size_t> & source,
            const std::optional<std::string> & name);
    
    std::optional<size_t> _sampler;
    std::optional<size_t> _source;
    std::optional<std::string> _name;

    bool operator==(const Texture & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const Texture & texture);

  class Sampler
  {
  public:
    Sampler(const Json::Value & samplerDocument);
    Sampler(const std::optional<GLenum> & magFilter,
            const std::optional<GLenum> & minFilter,
            GLenum wrapS,
            GLenum wrapT,
            const std::optional<std::string> & name);
    
    std::optional<GLenum> _magFilter;
    std::optional<GLenum> _minFilter;
    GLenum _wrapS;
    GLenum _wrapT;
    std::optional<std::string> _name;

    bool operator==(const Sampler & other) const;
  };
  
  std::ostream & operator<<(std::ostream & str, const Sampler & sampler);

  class Image
  {
  public:
    Image(const Json::Value & imageDocument);
    Image(const std::optional<std::string> & uri,
          const std::optional<std::string> & mimeType,
          const std::optional<size_t> & bufferView,
          const std::optional<std::string> & name);
    
    std::optional<std::string> _uri;
    std::optional<std::string> _mimeType;
    std::optional<size_t> _bufferView;
    std::optional<std::string> _name;

    bool operator==(const Image & other) const;
  };

  std::ostream & operator<<(std::ostream & str, const Image & image);
  
  class Asset
  {
  public:
    Asset(const std::string & gltfFile);
    
    size_t _scene;
    std::vector<Scene> _scenes;
    std::vector<Node> _nodes;
    std::vector<Mesh> _meshes;
    std::vector<Buffer> _buffers;
    std::vector<BufferView> _bufferViews;
    std::vector<Accessor> _accessors;
    std::vector<Material> _materials;
    std::vector<Texture> _textures;
    std::vector<Sampler> _samplers;
    std::vector<Image> _images;
  };
}

#endif
