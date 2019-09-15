#include "physicsystem.h"

#include "body.h"

PhysicSystem::PhysicSystem():
  _dispatcher(&_configuration),
  _world(&_dispatcher,
         &_overlappingPairCache,
         &_solver,
         &_configuration)
{
  _world.setGravity(btVector3(0, -10, 0));
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

void PhysicSystem::run(double delta)
{
  _world.stepSimulation(delta);
  for(auto && body : _bodies)
  {
    _moveSignal.emit(body.first, body.second->getWorldTransform());
  }
}

Signal<EntityId, glm::mat4> & PhysicSystem::onMove()
{
  return _moveSignal;
}

void PhysicSystem::move(EntityId entityId, const glm::vec3 & position)
{
  auto it = _bodies.find(entityId);
  if(it != _bodies.end())
  {
    it->second->move(position);
  }
}
