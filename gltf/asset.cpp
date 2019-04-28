#include "asset.h"

#include <fstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "json_utils.h"

namespace glm
{
  std::ostream& operator<<(std::ostream & str, const glm::mat4 & m)
  {
    return str << glm::to_string(m);
  }

  std::ostream& operator<<(std::ostream & str, const glm::vec3 & v)
  {
    return str << glm::to_string(v);
  }

  std::ostream& operator<<(std::ostream & str, const glm::quat & q)
  {
    return str << glm::to_string(q);
  }
}

namespace std
{  
  template<typename T>
  ostream & operator<<(ostream & str, const optional<T> & opt)
  {
    str << "{";
    if(opt)
      str << *opt;
    str << "}";
    return str;
  }

  template<typename T, typename U>
  ostream & operator<<(ostream & str, const map<T, U> & values)
  {
    str << "[";
    bool first = true;
    for(auto && item : values)
    {
      if(!first)
        str << ", ";
      str << item.first <<  " -> " << item.second;
      first = false;      
    }
    str << "]";
    return str;
  }
  
  template<typename T>
  ostream & operator<<(ostream & str, const vector<T> & values)
  {
    str << "[";
    bool first = true;
    for(auto && item : values)
    {
      if(!first)
        str << ", ";
      str << item;
      first = false;
    }
    str << "]";
    return str;
  }
}

namespace
{
  template<typename T>
  bool close(const T & left, const T & right)
  {
    auto d = (-right) + left;
    return glm::dot(d, d) < 0.01;
  }
  
  template<typename T>
  bool close(const std::optional<T> & left, const std::optional<T> & right)
  {
    if(left && right)
      return close(*left, *right);
    else
      return((left && right) || (!left && !right));
  }
}

////////////////////
// Scene
////////////////////

gltf::Scene::Scene(const Json::Value & sceneDocument)
{
  get(sceneDocument, "name", _name);
  get(sceneDocument, "nodes", _nodes);
}

gltf::Scene::Scene(const std::optional<std::string> & name,
                   const std::vector<size_t> & nodes):
  _name(name),
  _nodes(nodes)
{
}

bool gltf::Scene::operator==(const Scene & other) const
{
  return
    _name == other._name &&
    _nodes == other._nodes;
}

std::ostream & gltf::operator<<(std::ostream & str, const Scene & scene)
{
  str << "<"
      << scene._name << ", "
      << scene._nodes
      << ">";
  return str;
}

////////////////////
// Node
////////////////////

gltf::Node::Node(const Json::Value & nodeDocument)
{
  get(nodeDocument, "camera", _camera);
  get(nodeDocument, "children", _children);
  get(nodeDocument, "skin", _skin);
  get(nodeDocument, "matrix", _matrix);
  get(nodeDocument, "mesh", _mesh);
  get(nodeDocument, "rotation", _rotation);
  get(nodeDocument, "scale", _scale);
  get(nodeDocument, "translation", _translation);
  get(nodeDocument, "weights", _weights);
  get(nodeDocument, "name", _name);
}

gltf::Node::Node(const std::optional<size_t> & camera,
                 const std::vector<size_t> & children,
                 const std::optional<size_t> & skin,
                 const std::optional<glm::mat4> & matrix,
                 const std::optional<size_t> & mesh,
                 const std::optional<glm::quat> & rotation,
                 const std::optional<glm::vec3> & scale,
                 const std::optional<glm::vec3> & translation,
                 const std::vector<size_t> & weights,
                 const std::optional<std::string> & name):
  _camera(camera),
  _children(children),
  _skin(skin),
  _matrix(matrix),
  _mesh(mesh),
  _rotation(rotation),
  _scale(scale),
  _translation(translation),
  _weights(weights),
  _name(name)
{
}

bool gltf::Node::operator==(const Node & other) const
{
  return 
    _camera == other._camera &&
    _children == other._children &&
    _skin == other._skin &&
    _matrix == other._matrix &&
    _mesh == other._mesh &&
    close(_rotation, other._rotation) &&
    close(_scale, other._scale) &&
    close(_translation, other._translation) &&
    _weights == other._weights &&
    _name == other._name;    
}

std::ostream & gltf::operator<<(std::ostream & str, const Node & node)
{
  str << "<"
      << node._camera << ", "
      << node._children << ", "
      << node._skin << ", "
      << node._matrix << ", "
      << node._mesh << ", "
      << node._rotation << ", "
      << node._scale << ", "
      << node._translation << ", "
      << node._weights << ", "
      << node._name
      << ">";
  return str;
}

////////////////////
// Primitive
////////////////////

gltf::Primitive::Primitive(const Json::Value & primitiveDocument)
{
  get(primitiveDocument, "attributes", _attributes);
  get(primitiveDocument, "indices", _indices);
  get(primitiveDocument, "material", _material);
  get(primitiveDocument, "mode", _mode, GLenum(GL_TRIANGLES));
  get(primitiveDocument, "targets", _targets);
}

gltf::Primitive::Primitive(const std::map<std::string, size_t> & attributes,
                           const std::optional<size_t> & indices,
                           const std::optional<size_t> & material,
                           GLenum mode,
                           const std::vector<std::map<std::string, size_t> > & targets):
  _attributes(attributes),
  _indices(indices),
  _material(material),
  _mode(mode),
  _targets(targets)
{
}

bool gltf::Primitive::operator==(const Primitive & other) const
{
  return
    _attributes == other._attributes &&
    _indices == other._indices &&
    _material == other._material &&
    _mode == other._mode &&
    _targets == other._targets;
}

std::ostream & gltf::operator<<(std::ostream & str, const Primitive & primitive)
{
  str << "<"
      << primitive._attributes << ", "
      << primitive._indices << ", "
      << primitive._material << ", "
      << primitive._mode << ", "
      << primitive._targets
      << ">";
  return str;
}

////////////////////
// Mesh
////////////////////

gltf::Mesh::Mesh(const Json::Value & meshDocument)
{
  const Json::Value * primitivesDoc = getNode(meshDocument, "primitives");
  if(primitivesDoc)
  {
    for(auto && primitiveDoc : *primitivesDoc)
    {
      _primitives.push_back(Primitive(primitiveDoc));
    }
  }  
  get(meshDocument, "weights", _weights);
  get(meshDocument, "name", _name);
}

gltf::Mesh::Mesh(const std::vector<Primitive> & primitives,
                 const std::vector<size_t> & weights,
                 const std::optional<std::string> & name):
  _primitives(primitives),
  _weights(weights),
  _name(name)
{
}

bool gltf::Mesh::operator==(const Mesh & other) const
{
  return
    _primitives == other._primitives &&
    _weights == other._weights &&
    _name == other._name;
}

std::ostream & gltf::operator<<(std::ostream & str, const Mesh & mesh)
{
  str << "<"
      << mesh._primitives << ", "
      << mesh._weights << ", "
      << mesh._name
      << ">";
  return str;
}

////////////////////
// Asset
////////////////////

gltf::Asset::Asset(const std::string & gltfFile)
{
  std::filesystem::path modelPath = std::filesystem::path(gltfFile).parent_path();
  std::ifstream str(gltfFile);
  Json::Value document;
  str >> document;
  
  // Scene
  get(document, "scene", _scene);
  
  // Scenes
  const Json::Value * scenesDoc = getNode(document, "scenes");
  if(scenesDoc)
  {
    for(auto && sceneDoc : *scenesDoc)
    {
      _scenes.push_back(Scene(sceneDoc));
    }
  }

  // Nodes
  const Json::Value * nodesDoc = getNode(document, "nodes");
  if(nodesDoc)
  {
    for(auto && nodeDoc : *nodesDoc)
    {
      _nodes.push_back(Node(nodeDoc));
    }
  }

  // Meshes
  const Json::Value * meshesDoc = getNode(document, "meshes");
  if(meshesDoc)
  {
    for(auto && meshDoc : *meshesDoc)
    {
      _meshes.push_back(Mesh(meshDoc));
    }
  }
}
