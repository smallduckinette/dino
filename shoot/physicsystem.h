#ifndef __SHOOT_PHYSICSYSTEM_H__
#define __SHOOT_PHYSICSYSTEM_H__

#include "entity/entityid.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class PhysicSystem
{
public:
  PhysicSystem();
  PhysicSystem(const PhysicSystem &) = delete;
  PhysicSystem & operator=(const PhysicSystem &) = delete;
  
  void add(EntityId entityId);

private:
  btDefaultCollisionConfiguration _configuration;
  btCollisionDispatcher _dispatcher;
  btDbvtBroadphase _overlappingPairCache;
  btSequentialImpulseConstraintSolver _solver;
  btDiscreteDynamicsWorld _world;
};

#endif
