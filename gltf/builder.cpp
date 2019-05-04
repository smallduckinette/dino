#include "builder.h"

#include "adh/node.h"
#include "adh/transform.h"
#include "adh/primitive.h"
#include "asset.h"

gltf::Builder::Builder(const std::string & gltfFile):
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

  //for(auto && primitive : mesh._primitives)
  {
    auto primitiveNode = std::make_shared<adh::Primitive>();
    
    node->addChild(primitiveNode);
  }
  
  return node;
}
