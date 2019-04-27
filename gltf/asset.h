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


namespace gltf
{
  class Scene
  {
  public:
    Scene(const Json::Value & sceneDocument);
    
    std::optional<std::string> _name;
    std::vector<size_t> _nodes;
  };

  class Node
  {
  public:
    Node(const Json::Value & nodeDocument);

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
  };
  
  class Asset
  {
  public:
    Asset(const std::string & gltfFile);
    
    size_t _scene;
    std::vector<Scene> _scenes;
    std::vector<Node> _nodes;
  };
}

#endif
