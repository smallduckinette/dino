#ifndef __GLTF_JSON_UTILS_H__
#define __GLTF_JSON_UTILS_H__

#include <string>
#include <optional>
#include <vector>
#include <json/json.h>
#include <glm/glm.hpp>

namespace gltf
{
  namespace detail
  {
    void parse(const std::string & label,
               const Json::Value * node,
               size_t & value);
    
    void parse(const std::string & label,
               const Json::Value * node,
               std::string & value);

    void parse(const std::string & label,
               const Json::Value * node,
               float & value);
  }
  
  const Json::Value * getNode(const Json::Value & document,
                              const std::string & name);
  
  void get(const Json::Value & document,
           const std::string & name,
           glm::mat4 & value);
  
  void get(const Json::Value & document,
           const std::string & name,
           glm::quat & value);

  void get(const Json::Value & document,
           const std::string & name,
           glm::vec3 & value);
  
  template<typename T>
  void get(const Json::Value & document,
           const std::string & name,
           T & value)
  {
    auto node = getNode(document, name);
    if(node)
    {
      detail::parse(name, node, value);
    }
    else
    {
      throw std::runtime_error("Could not find node " + name + " in document");
    }
  }
  
  template<typename T>
  void get(const Json::Value & document,
           const std::string & name,
           std::optional<T> & value)
  {
    auto node = getNode(document, name);
    if(node)
    {
      value = T();
      get(document, name, *value);
    }
    else
    {
      value = {};
    }
  }

  template<typename T>
  void get(const Json::Value & document,
           const std::string & name,
           std::vector<T> & values)
  {
    auto node = getNode(document, name);
    if(node)
    {
      for(auto && item : *node)
      {
        T value;
        detail::parse("array " + name, &item, value);
        values.push_back(value);
      }
    }
  }
}

#endif
