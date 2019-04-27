#include "asset.h"

#include <fstream>

#include "json_utils.h"

gltf::Scene::Scene(const Json::Value & sceneDocument)
{
  get(sceneDocument, "name", _name);
  get(sceneDocument, "nodes", _nodes);
}

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
}
