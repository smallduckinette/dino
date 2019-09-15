#ifndef __ENTITY_ENTITYFACTORY_H__
#define __ENTITY_ENTITYFACTORY_H__

#include <map>
#include <string>
#include <filesystem>
#include <json/json.h>
#include "entityid.h"

class System;

class EntityFactory
{
public:
  EntityFactory(const std::string & entityFile);
  
  void registerSystem(const std::string & name,
                      System * system);
  
  EntityId addEntity(const std::string & entity);
  
private:
  Json::Value _doc;
  EntityIdGenerator _entityIdGenerator;
  std::map<std::string, System *> _systems;
  std::filesystem::path _rootDirectory;
};

#endif
