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
    Node() {}
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
