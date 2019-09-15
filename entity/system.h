#ifndef __ENTITY_SYSTEM_H__
#define __ENTITY_SYSTEM_H__

#include <string>
#include <filesystem>
#include <json/json.h>
#include "entityid.h"

class System
{
public:
  virtual ~System() = default;
  
  virtual void init(const std::filesystem::path & rootDirectory) = 0;
  
  virtual void add(EntityId entityId,
                   const Json::Value & doc) = 0;
};

#endif
