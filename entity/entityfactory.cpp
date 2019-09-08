#include "entityfactory.h"

#include "core/json_utils.h"
#include "system.h"

void EntityFactory::registerSystem(const std::string & name, System * system)
{
  _systems.insert({name, system});
}

EntityId EntityFactory::addEntity(const Json::Value & doc)
{
  EntityId entityId = _entityIdGenerator.generate();
  for(auto && member : doc.getMemberNames())
  {
    auto it = _systems.find(member);
    if(it != _systems.end())
    {
      it->second->add(entityId, core::getNodeOrThrow(doc, member));
    }
  }
  return entityId;
}
