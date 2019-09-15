#include "physicsystem.h"

#include "body.h"

PhysicSystem::PhysicSystem():
  _dispatcher(&_configuration),
  _world(&_dispatcher,
         &_overlappingPairCache,
         &_solver,
         &_configuration)
{
}

void PhysicSystem::init(const std::filesystem::path &)
{
}

void PhysicSystem::add(EntityId entityId, const Json::Value & doc)
{
  _bodies.insert({entityId, std::make_shared<Body>(doc,
                                                   btVector3(0, 0, 0),
                                                   btVector3(0, 0, 0),
                                                   &_world)});
}

