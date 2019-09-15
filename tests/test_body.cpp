#include <boost/test/unit_test.hpp>

#include <fstream>
#include "shoot/body.h"

class PhysicsFixture
{
public:
  PhysicsFixture():
    dispatcher(&collisionConfiguration),
    world(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
  {
  }    
  
  btDefaultCollisionConfiguration collisionConfiguration;
  btCollisionDispatcher dispatcher;
  btDbvtBroadphase overlappingPairCache;
  btSequentialImpulseConstraintSolver solver;
  btDiscreteDynamicsWorld world;
};

BOOST_FIXTURE_TEST_SUITE(Physics, PhysicsFixture)


BOOST_AUTO_TEST_CASE(testLoadSphere)
{
  std::ifstream str("data/body_sphere.json");
  Json::Value doc;
  str >> doc;
  
  Body body(doc, btVector3(0, 10, 0), btVector3(0, 0, 1), &world);
}

BOOST_AUTO_TEST_CASE(testSphereFreeFall)
{
  world.setGravity(btVector3(0, -10, 0));

  std::ifstream str("data/body_sphere.json");
  Json::Value doc;
  str >> doc;
  
  Body body(doc, btVector3(0, 10, 0), btVector3(0, 0, 1), &world);
  
  // Go ahead by 1 second
  for(size_t index = 0; index < 60; ++index)
  {
    world.stepSimulation(1.0f/60.0f);
  }
  
  //auto trans = body.getWorldTransform();
  //BOOST_CHECK(trans.getOrigin().distance2(btVector3(0, 5.08, 0.983)) < 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
