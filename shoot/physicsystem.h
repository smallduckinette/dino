#ifndef __SHOOT_PHYSICSYSTEM_H__
#define __SHOOT_PHYSICSYSTEM_H__

#include <memory>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "entity/system.h"

class Body;

class PhysicSystem : public System
{
public:
  PhysicSystem();
  PhysicSystem(const PhysicSystem &) = delete;
  PhysicSystem & operator=(const PhysicSystem &) = delete;
  
  void add(EntityId entityId, const Json::Value & doc) override;

private:
  btDefaultCollisionConfiguration _configuration;
  btCollisionDispatcher _dispatcher;
  btDbvtBroadphase _overlappingPairCache;
  btSequentialImpulseConstraintSolver _solver;
  btDiscreteDynamicsWorld _world;
  
  std::map<EntityId, std::shared_ptr<Body> > _bodies;
};

#endif
