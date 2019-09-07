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

BOOST_AUTO_TEST_SUITE_END()
