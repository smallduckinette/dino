#include "physicsystem.h"


PhysicSystem::PhysicSystem():
  _dispatcher(&_configuration),
  _world(&_dispatcher,
         &_overlappingPairCache,
         &_solver,
         &_configuration)
{
}

void PhysicSystem::add(EntityId)
{
}
