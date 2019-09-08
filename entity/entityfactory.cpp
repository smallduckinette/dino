#include "entityfactory.h"

#include <fstream>

#include "core/json_utils.h"
#include "system.h"

EntityFactory::EntityFactory(const std::string & entityDefinition)
{
  std::ifstream str(entityDefinition);
  str >> _doc;
}

void EntityFactory::registerSystem(const std::string & name, System * system)
{
  _systems.insert({name, system});
}

EntityId EntityFactory::addEntity(const std::string & entity)
{
  auto entityDoc = core::getNodeOrThrow(_doc, entity);
  
  EntityId entityId = _entityIdGenerator.generate();
  for(auto && member : entityDoc.getMemberNames())
  {
    auto it = _systems.find(member);
    if(it != _systems.end())
    {
      it->second->add(entityId, core::getNodeOrThrow(entityDoc, member));
    }
  }
  return entityId;
}
