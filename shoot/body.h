#ifndef __SHOOT_BODY_H__
#define __SHOOT_BODY_H__

#include <memory>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <json/json.h>

class Body
{
public:
  Body(const Json::Value & doc,
       const btVector3 & position,
       const btVector3 & velocity,
       btDiscreteDynamicsWorld * world);
  ~Body();
  
  btTransform getWorldTransform() const;
  
private:
  std::unique_ptr<btCollisionShape> makeShape(const Json::Value & doc) const;
  float getMass(const Json::Value & doc) const;
  btVector3 calculateInertia(const Json::Value & doc) const;
  btTransform getTransform(const btVector3 & position) const;
  float getRestitution(const Json::Value & doc) const;
  
  std::unique_ptr<btCollisionShape> _shape;
  btDefaultMotionState _motionState;
  btRigidBody _body;
  btDiscreteDynamicsWorld * _world;
};

#endif
