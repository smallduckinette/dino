#ifndef __GLTF_BUFFER_H__
#define __GLTF_BUFFER_H__

#include <filesystem>
#include <json/json.h>

namespace gltf
{
  class OldBuffer
  {
  public:
    OldBuffer(const std::filesystem::path & modelPath,
              const Json::Value & doc);
    
    const char * getData() const;
    size_t getSize() const;
    
  private:
    std::string _data;
  };  
}

#endif
