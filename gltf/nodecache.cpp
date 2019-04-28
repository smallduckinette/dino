#include "nodecache.h"

#include <numeric>
#include <fstream>
#include <boost/log/trivial.hpp>

#include "adh/primitive.h"
#include "adh/texture.h"
#include "adh/material.h"
#include "adh/shader.h"
#include "adh/texturecolor.h"
#include "adh/plaincolor.h"
#include "adh/transform.h"

#include "accessor.h"
#include "bufferview.h"
#include "buffer.h"


gltf::NodeCache::NodeCache(const std::string & shaderDir,
                           const std::string & gltfFile):
  _shaderPath(std::filesystem::path(shaderDir)),
  _modelPath(std::filesystem::path(gltfFile).parent_path())
{
  std::ifstream str(gltfFile);
  str >> _document;
}

std::shared_ptr<adh::Node> gltf::NodeCache::getScene()
{
  auto sceneIndex = _document["scene"].asUInt();
  auto scene = _document["scenes"][Json::ArrayIndex(sceneIndex)];
  auto object = std::make_shared<adh::Node>(scene["name"].asString());
  
  for(auto && nodeIndex : scene["nodes"])
  {
    auto && node = _document["nodes"][Json::ArrayIndex(nodeIndex.asUInt())];
    if(node.isMember("mesh")) // Ignore non-mesh nodes
    {
      // Create a transform node
      auto transform = std::make_shared<adh::Transform>();
      object->addChild(transform);
      transform->addChild(getMesh(node["mesh"].asUInt()));

      // Apply all the transformations we can find
      if(node.isMember("translation"))
      {
        transform->setTranslate(glm::vec3(node["translation"][0].asFloat(),
                                          node["translation"][1].asFloat(),
                                          node["translation"][2].asFloat()));
      }
      if(node.isMember("rotation"))
      {
        transform->setRotate(glm::quat(node["rotation"][0].asFloat(),
                                       node["rotation"][1].asFloat(),
                                       node["rotation"][2].asFloat(),
                                       node["rotation"][3].asFloat()));
      }
      if(node.isMember("scale"))
      {
        transform->setScale(glm::vec3(node["scale"][0].asFloat(),
                                      node["scale"][1].asFloat(),
                                      node["scale"][2].asFloat()));
      }
    }
  }
  
  return object;
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

std::shared_ptr<adh::Node> gltf::NodeCache::getPrimitive(const Json::Value & primitiveDoc)
{
  std::vector<std::shared_ptr<Accessor> > selectedAccessors;
  selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("POSITION", "").asUInt()));
  selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("NORMAL", "").asUInt()));
  selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("TEXCOORD_0", "").asUInt()));
  
  if(primitiveDoc["attributes"].isMember("TANGENT"))
  {
    selectedAccessors.push_back(getAccessor(primitiveDoc["attributes"].get("TANGENT", "").asUInt()));
  }
  
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
  
  // Wrap the primitive in the material
  auto material = getMaterial(primitiveDoc.get("material", "").asUInt());
  material->addChild(primitive);
  
  return material;
}

std::shared_ptr<adh::Shader> gltf::NodeCache::getShader(const std::string & shaderName,
                                                        const std::vector<std::string> & defines)
{
  auto key = std::make_pair(shaderName, defines);
  auto it = _shaderCache.find(key);
  if(it != _shaderCache.end())
  {
    return it->second;
  }
  else
  {
    std::fstream vertex(_shaderPath / (shaderName + ".vert"));
    std::fstream fragment(_shaderPath / (shaderName + ".frag"));
    auto shader = std::make_shared<adh::Shader>(vertex,
                                                fragment,
                                                defines);
    
    return _shaderCache.insert({key, shader}).first->second;
  }
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

std::shared_ptr<gltf::OldBufferView> gltf::NodeCache::getBufferView(size_t index)
{
  auto it = _bufferViewCache.find(index);
  if(it != _bufferViewCache.end())
  {
    return it->second;
  }
  else
  {
    auto && bufferViewDoc = _document["bufferViews"][Json::ArrayIndex(index)];
    
    return _bufferViewCache.insert({index, std::make_shared<OldBufferView>(bufferViewDoc, *this)}).first->second;
  }    
}

std::shared_ptr<gltf::OldBuffer> gltf::NodeCache::getBuffer(size_t index)
{
  auto it = _bufferCache.find(index);
  if(it != _bufferCache.end())
  {
    return it->second;
  }
  else
  {
    auto && bufferDoc = _document["buffers"][Json::ArrayIndex(index)];
    
    return _bufferCache.insert({index, std::make_shared<OldBuffer>(_modelPath,
                                                                   bufferDoc)}).first->second;
  }    
}

std::shared_ptr<adh::Texture> gltf::NodeCache::getTexture(size_t index)
{
  auto it = _textureCache.find(index);
  if(it != _textureCache.end())
  {
    return it->second;
  }
  else
  {
    auto && textureDoc = _document["textures"][Json::ArrayIndex(index)];
    auto && imageDoc = _document["images"][Json::ArrayIndex(textureDoc.get("source", "").asUInt())];
    std::string name = imageDoc.get("name", "").asString();
    std::string uri = imageDoc.get("uri", "").asString();
    
    return _textureCache.insert({index, std::make_shared<adh::Texture>(name,
                                                                       _modelPath / uri)}).first->second;
  }
}

std::shared_ptr<adh::Material> gltf::NodeCache::getMaterial(size_t index)
{
  auto it = _materialCache.find(index);
  if(it != _materialCache.end())
  {
    return it->second;
  }
  else
  {
    auto && materialDoc= _document["materials"][Json::ArrayIndex(index)];
    std::string name = materialDoc.get("name", "").asString();
    
    auto && pbrDoc = materialDoc["pbrMetallicRoughness"];
    std::vector<std::shared_ptr<adh::Color> > colors;
    std::vector<std::string> defines;

    if(pbrDoc.isMember("baseColorTexture"))
    {
      size_t diffuseTextureIndex = pbrDoc["baseColorTexture"].get("index", "").asUInt();
      auto diffuseTexture = getTexture(diffuseTextureIndex);
      auto diffuseColor = std::make_shared<adh::TextureColor>(diffuseTexture,
                                                              GL_TEXTURE0,
                                                              "diffuseMap");
      colors.push_back(diffuseColor);
      defines.push_back("HAS_DIFFUSE_TEXTURE");
    }
    else if(pbrDoc.isMember("baseColorFactor"))
    {
      auto diffuse = glm::vec4(pbrDoc["baseColorFactor" ][0].asFloat(),
                               pbrDoc["baseColorFactor" ][1].asFloat(),
                               pbrDoc["baseColorFactor" ][2].asFloat(),
                               pbrDoc["baseColorFactor"][3].asFloat());
      auto diffuseColor = std::make_shared<adh::PlainColor>("diffuseColor",
                                                            diffuse);
      colors.push_back(diffuseColor);
    }
    else throw std::runtime_error("Missing diffuse color information");
    
    if(pbrDoc.isMember("metallicRoughnessTexture"))
    {
      size_t metalRoughnessIndex = materialDoc["pbrMetallicRoughness"]["metallicRoughnessTexture"].get("index", "").asUInt();
      auto metalRoughnessTexture = getTexture(metalRoughnessIndex);
      auto metalRoughnessColor = std::make_shared<adh::TextureColor>(metalRoughnessTexture,
                                                                     GL_TEXTURE2,
                                                                     "metalroughnessMap");
      colors.push_back(metalRoughnessColor);
      defines.push_back("HAS_METALROUGHNESS_TEXTURE");
    }
    else if(pbrDoc.isMember("metallicFactor") && pbrDoc.isMember("roughnessFactor"))
    {
      auto metal = pbrDoc["metallicFactor"].asFloat();
      auto roughness = pbrDoc["roughnessFactor"].asFloat();
      auto metalRoughness = glm::vec4(roughness, metal, 0, 0);
      auto metalRoughnessColor = std::make_shared<adh::PlainColor>("mr",
                                                                   metalRoughness);
      colors.push_back(metalRoughnessColor);
    }
    else throw std::runtime_error("Missing metal roughness information");
    
    if(materialDoc.isMember("normalTexture"))
    {
      size_t normalTextureIndex = materialDoc["normalTexture"].get("index", "").asUInt();
      auto normalTexture = getTexture(normalTextureIndex);
      auto normalColor = std::make_shared<adh::TextureColor>(normalTexture,
                                                             GL_TEXTURE1,
                                                             "normalMap");
      colors.push_back(normalColor);
      defines.push_back("HAS_NORMAL_TEXTURE");
    }
    // Else do nothing
    
    auto shader = getShader("pbr", defines);
    
    return _materialCache.insert({index, std::make_shared<adh::Material>(name,
                                                                         shader,
                                                                         colors)}).first->second;
  }  
}
