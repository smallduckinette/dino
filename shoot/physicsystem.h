#ifndef __SHOOT_PHYSICSYSTEM_H__
#define __SHOOT_PHYSICSYSTEM_H__

#include <memory>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "entity/system.h"
#include "entity/signal.h"

class Body;

class PhysicSystem : public System
{
public:
  PhysicSystem();
  PhysicSystem(const PhysicSystem &) = delete;
  PhysicSystem & operator=(const PhysicSystem &) = delete;

  void init(const std::filesystem::path &) override;
  void add(EntityId entityId, const Json::Value & doc) override;
  
  void run(double delta);
  
  Signal<EntityId, glm::mat4> & onMove();
  
  void move(EntityId entityId, const glm::vec3 & position);
  
private:
  btDefaultCollisionConfiguration _configuration;
  btCollisionDispatcher _dispatcher;
  btDbvtBroadphase _overlappingPairCache;
  btSequentialImpulseConstraintSolver _solver;
  btDiscreteDynamicsWorld _world;
  
  std::map<EntityId, std::shared_ptr<Body> > _bodies;
  
  Signal<EntityId, glm::mat4> _moveSignal;
};

#endif
