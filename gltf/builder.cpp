#include "builder.h"

#include <fstream>

#include "adh/node.h"
#include "adh/transform.h"
#include "adh/primitive.h"
#include "adh/shader.h"
#include "asset.h"

gltf::Builder::Builder(const std::string & shaderPath,
                       const std::string & gltfFile):
  _shaderPath(std::filesystem::path(shaderPath)),
  _asset(std::make_shared<Asset>(gltfFile))
{
}

std::unique_ptr<adh::Node> gltf::Builder::build() const
{
  auto && scene = _asset->_scenes.at(_asset->_scene);
  
  auto sceneNode = std::make_unique<adh::Node>(scene._name.value_or(std::string()));
  
  for(auto && nodeIndex : scene._nodes)
  {
    sceneNode->addChild(buildNode(nodeIndex));
  }
  
  return sceneNode;
}

std::unique_ptr<adh::Node> gltf::Builder::buildNode(size_t nodeIndex) const
{
  // Create the node
  auto && node = _asset->_nodes.at(nodeIndex);
  auto transform = std::make_unique<adh::Transform>(node._name.value_or(std::string()));

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
    transform->addChild(buildNode(childIndex));
  }

  // Build the mesh if present
  if(node._mesh)
  {
    transform->addChild(buildMesh(*node._mesh));
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
    
    primitiveNode->setShader(getShader(defines));
    node->addChild(primitiveNode);
    
    primitiveNode->unbind();
  }
  
  return node;
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
