#include "body.h"

#include <glm/gtc/type_ptr.hpp>
#include "core/json_utils.h"

Body::Body(const Json::Value & doc,
           const btVector3 & position,
           const btVector3 & velocity,
           btDiscreteDynamicsWorld * world):
  _shape(makeShape(doc)),
  _motionState(getTransform(position)),
  _body(btRigidBody::btRigidBodyConstructionInfo(getMass(doc),
                                                 &_motionState,
                                                 _shape.get(),
                                                 calculateInertia(doc))),
  _world(world)
{
  _body.setRestitution(getRestitution(doc));
  _body.setLinearVelocity(velocity);
  _world->addRigidBody(&_body);
}

Body::~Body()
{
  _world->removeRigidBody(&_body);
}

glm::mat4 Body::getWorldTransform() const
{
  btTransform trans;
  _motionState.getWorldTransform(trans);
  float rawMatrix[16];
  trans.getOpenGLMatrix(rawMatrix);
  return glm::make_mat4(rawMatrix);
}

std::unique_ptr<btCollisionShape> Body::makeShape(const Json::Value & doc) const
{
  auto node = core::getNodeOrThrow(doc, "shape");
  std::string type;
  core::get(node, "type", type);
  if(type == "sphere")
  {
    float radius;
    core::get(node, "radius", radius);
    return std::make_unique<btSphereShape>(radius);
  }
  else if(type == "box")
  {
    glm::vec3 dim;
    core::get(node, "dimensions", dim);
    return std::make_unique<btBoxShape>(btVector3(dim.x, dim.y, dim.z));
  }
  else
    throw std::runtime_error("Unknown shape type " + type);
}

float Body::getMass(const Json::Value & doc) const
{
  float mass = 0;
  core::get(doc, "mass", mass, 0.f);
  return mass;
}

btVector3 Body::calculateInertia(const Json::Value & doc) const
{
  btVector3 inertia(0, 0, 0);
  _shape->calculateLocalInertia(getMass(doc), inertia);
  return inertia;
}

btTransform Body::getTransform(const btVector3 & position) const
{
  btTransform transform;
  transform.setIdentity();
  transform.setOrigin(position);
  return transform;
}

float Body::getRestitution(const Json::Value & doc) const
{
  float restitution = 0;
  core::get(doc, "restitution", restitution, 0.f);
  return restitution;
}

void Body::move(const glm::vec3 & position)
{
  btTransform transform;
  transform.setIdentity();
  transform.setOrigin(btVector3(position.x, position.y, position.z));
  _motionState.setWorldTransform(transform);
  _body.setWorldTransform(transform);
}
