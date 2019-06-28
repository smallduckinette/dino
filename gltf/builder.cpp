#include "builder.h"

#include <fstream>

#include "adh/node.h"
#include "adh/transform.h"
#include "adh/primitive.h"
#include "adh/shader.h"
#include "adh/texture.h"
#include "adh/animation.h"
#include "adh/channel.h"
#include "asset.h"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL


gltf::Builder::Builder(const std::shared_ptr<adh::Clock> & clock,
                       const std::string & shaderPath,
                       const std::string & gltfFile):
  _clock(clock),
  _shaderPath(std::filesystem::path(shaderPath)),
  _asset(std::make_shared<Asset>(gltfFile)),
  _modelPath(std::filesystem::path(gltfFile).parent_path())
{
}

std::unique_ptr<adh::Node> gltf::Builder::build(std::vector<std::unique_ptr<adh::Animation> > & animations) const
{
  auto && scene = _asset->_scenes.at(_asset->_scene);
  
  auto sceneNode = std::make_unique<adh::Node>(scene._name.value_or(std::string()));
  
  std::map<size_t, std::shared_ptr<adh::Transform> > animationNodes;

  // Build all the nodes for the scene
  for(auto && nodeIndex : scene._nodes)
  {
    sceneNode->addChild(buildNode(nodeIndex, animationNodes));
  }
  
  // Build all the animations for the scene
  for(auto && animation : _asset->_animations)
  {
    auto && animationControl =
      std::make_unique<adh::Animation>(animation._name.value_or(std::string()),
                                       _clock);
    for(auto && channel : animation._channels)
    {
      auto && sampler = animation._samplers.at(channel._sampler);
      if(!channel._target._node)
        throw std::runtime_error("Animation does not have target node");
      auto targetNodeIt = animationNodes.find(*channel._target._node);
      if(targetNodeIt == animationNodes.end())
        throw std::runtime_error("Animation targets unknown node");
      
      if(channel._target._path == "translation")
      {
        animationControl->addChannel
          (std::make_unique<adh::InterpolatedChannel<glm::vec3> >
           (buildInterpolator<glm::vec3>(sampler),
            targetNodeIt->second,
            [](adh::Transform * transform, const glm::vec3 & t)
            {
              transform->setTranslate(t);
            }));
      }
      else if(channel._target._path == "rotation")
      {
        animationControl->addChannel
          (std::make_unique<adh::InterpolatedChannel<glm::quat> >
           (buildInterpolator<glm::quat>(sampler),
            targetNodeIt->second,
            [](adh::Transform * transform, const glm::quat & t)
            {
              transform->setRotate(t);
            }));
      }
      else if(channel._target._path == "scale")
      {
        animationControl->addChannel
          (std::make_unique<adh::InterpolatedChannel<glm::vec3> >
           (buildInterpolator<glm::vec3>(sampler),
            targetNodeIt->second,
            [](adh::Transform * transform, const glm::vec3 & t)
            {
              transform->setScale(t);
            }));        
      }
    }
    
    animations.push_back(std::move(animationControl));
  }
  
  return sceneNode;
}

std::shared_ptr<adh::Node> gltf::Builder::buildNode(size_t nodeIndex, std::map<size_t, std::shared_ptr<adh::Transform> > & animationNodes) const
{
  // Create the node
  auto && node = _asset->_nodes.at(nodeIndex);
  auto transform = std::make_shared<adh::Transform>(node._name.value_or(std::string()));
  auto animation = std::make_shared<adh::Transform>(node._name.value_or(std::string()) + " animation");
  transform->addChild(animation);
  animationNodes.insert({nodeIndex, animation});
  
  // Configure the transformation
  if(node._matrix)
    transform->setMatrix(*node._matrix);

  if(node._rotation)
    transform->setRotate(*node._rotation);

  if(node._scale)
    transform->setScale(*node._scale);

  if(node._translation)
    transform->setTranslate(*node._translation);

  // Recursively build children
  for(auto && childIndex : node._children)
  {
    animation->addChild(buildNode(childIndex, animationNodes));
  }

  // Build the mesh if present
  if(node._mesh)
  {
    animation->addChild(buildMesh(*node._mesh));
  }
  
  return transform;
}

std::unique_ptr<adh::Node> gltf::Builder::buildMesh(size_t meshIndex) const
{
  auto && mesh = _asset->_meshes.at(meshIndex);
  auto node = std::make_unique<adh::Node>(mesh._name.value_or(std::string()));

  for(auto && primitive : mesh._primitives)
  {
    auto primitiveNode = std::make_shared<adh::Primitive>(primitive._mode);
    std::vector<std::string> defines;
    
    primitiveNode->bind();
    
    if(primitive._indices)
    {
      setIndicesBuffer(primitiveNode, _asset->_accessors.at(*primitive._indices));
    }

    for(auto && attr : primitive._attributes)
    {
      auto && attributeName = attr.first;
      auto && accessor = _asset->_accessors.at(attr.second);
      
      size_t index = setDataBuffer(primitiveNode, accessor);
      defines.push_back("HAS_" + attributeName);
      defines.push_back(attributeName + " " + std::to_string(index));
      
    }
    
    if(primitive._material)
    {
      auto && material = _asset->_materials.at(*primitive._material);
      if(material._pbrMetallicRoughness)
      {
        auto && pbr = *material._pbrMetallicRoughness;
        primitiveNode->setColor("diffuseColor", pbr._baseColorFactor);
        if(pbr._baseColorTexture)
        {
          primitiveNode->setTexture(buildTexture("diffuseMap", pbr._baseColorTexture->_index),
                                    GL_TEXTURE0);
          defines.push_back("HAS_DIFFUSE_TEXTURE");
        }
        primitiveNode->setColor("mr", glm::vec4(0, pbr._roughnessFactor, pbr._metallicFactor, 0));
        if(pbr._metallicRoughnessTexture)
        {
          primitiveNode->setTexture(buildTexture("metalroughnessMap", pbr._metallicRoughnessTexture->_index),
                                    GL_TEXTURE2);
          defines.push_back("HAS_METALROUGHNESS_TEXTURE");          
        }
      }
      if(material._normalTexture)
      {
        primitiveNode->setTexture(buildTexture("normalMap", material._normalTexture->_index),
                                  GL_TEXTURE1);
        defines.push_back("HAS_NORMAL_TEXTURE");
      }
    }
    
    primitiveNode->setShader(getShader(defines));
    node->addChild(primitiveNode);
    
    primitiveNode->unbind();
  }
  
  return node;
}

std::unique_ptr<adh::Texture> gltf::Builder::buildTexture(const std::string & name,
                                                          size_t textureIndex) const
{
  auto && texture = _asset->_textures.at(textureIndex);
  if(!texture._source)
    throw std::runtime_error("No source for texture " + std::to_string(textureIndex));
  
  auto && image = _asset->_images.at(*texture._source);
  
  if(!image._uri)
    throw std::runtime_error("No image uri in image " + std::to_string(*texture._source));

  return std::make_unique<adh::Texture>(name, _modelPath / (*image._uri));
}

void gltf::Builder::setIndicesBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                                     const gltf::Accessor & accessor) const
{
  if(accessor._bufferView)
  {
    auto && bufferView = _asset->_bufferViews.at(*accessor._bufferView);
    auto && buffer = _asset->_buffers.at(bufferView._buffer);
    
    const char * data = buffer._data.data() + bufferView._byteOffset;
    size_t length = bufferView._byteLength;
    
    primitiveNode->setIndicesBuffer(data, length,
                                    accessor._count, accessor._componentType);
  }
}

size_t gltf::Builder::setDataBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                                    const gltf::Accessor & accessor) const
{
  if(accessor._bufferView)
  {
    auto && bufferView = _asset->_bufferViews.at(*accessor._bufferView);
    auto && buffer = _asset->_buffers.at(bufferView._buffer);
    
    const char * data = buffer._data.data() + bufferView._byteOffset;
    size_t length = bufferView._byteLength;
    
    return primitiveNode->setDataBuffer(data,
                                        length,
                                        accessor.getComponentCount(),
                                        accessor._componentType,
                                        accessor._normalized,
                                        bufferView._byteStride,
                                        accessor._byteOffset);
  }
  else
    return 0;
}

std::shared_ptr<adh::Shader> gltf::Builder::getShader(const std::vector<std::string> & defines) const
{
  std::fstream vertex(_shaderPath / "pbr.vert");
  std::fstream fragment(_shaderPath / "pbr.frag");
  return std::make_shared<adh::Shader>(vertex,
                                       fragment,
                                       defines);
}

template<typename T>
std::unique_ptr<adh::Interpolator<T> > gltf::Builder::buildInterpolator(const AnimationSampler & sampler) const
{
  auto && tsAccessor = _asset->_accessors.at(sampler._input);
  if(!tsAccessor._bufferView)
    throw std::runtime_error("No buffer view for animation accessor");
  auto && tsBufferView = _asset->_bufferViews.at(*tsAccessor._bufferView);
  auto && tsBuffer = _asset->_buffers.at(tsBufferView._buffer);
  
  const char * tsData = tsBuffer._data.data() + tsBufferView._byteOffset;
  const float * tsFloatData = reinterpret_cast<const float *>(tsData);
  
  auto && valAccessor = _asset->_accessors.at(sampler._output);
  if(!valAccessor._bufferView)
    throw std::runtime_error("No buffer view for animation accessor");
  auto && valBufferView = _asset->_bufferViews.at(*valAccessor._bufferView);
  auto && valBuffer = _asset->_buffers.at(valBufferView._buffer);
  
  const char * valData = valBuffer._data.data() + valBufferView._byteOffset;
  const T * valFloatData = reinterpret_cast<const T *>(valData);
  
  if(sampler._interpolation == "LINEAR")
  {
    std::map<float, T> values;
    
    for(size_t index = 0; index < tsAccessor._count; ++index)
    {
      values.insert({tsFloatData[index], valFloatData[index]});
    }
    
    return std::make_unique<adh::LinearInterpolator<T> >(values);
  }
  else if(sampler._interpolation == "CUBICSPLINE")
  {
    std::map<float, std::tuple<T, T, T> > values;
    
    for(size_t index = 0; index < tsAccessor._count; ++index)
    {
      values.insert({tsFloatData[index],
          {
            valFloatData[index + tsAccessor._count],
              valFloatData[index],
              valFloatData[index + 2 * tsAccessor._count]
              }});
    }
    
    for(auto && val : values)
    {
      std::cout << val.first
                << " " << glm::to_string(std::get<0>(val.second))
                << " " << glm::to_string(std::get<1>(val.second))
                << " " << glm::to_string(std::get<2>(val.second))
                << std::endl;
    }

    return std::make_unique<adh::CubicSplineInterpolator<T> >(values);
  }
  else
    throw std::runtime_error("Unknown interpolation type " + sampler._interpolation);
}
