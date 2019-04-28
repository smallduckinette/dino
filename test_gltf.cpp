#include <boost/test/unit_test.hpp>

#include <iostream>
#include "gltf/asset.h"


BOOST_AUTO_TEST_SUITE(Gltf)

BOOST_AUTO_TEST_CASE(testLoadSimpleAsset)
{
  gltf::Asset asset("../models/samples/plain.gltf");

  // Scene
  {
    BOOST_TEST(0 == asset._scene);
  }
  
  // Scenes
  {
    std::vector<gltf::Scene> expected{
      {
        gltf::Scene("Scene", {0, 1, 2})
          }};
    
    BOOST_TEST(expected == asset._scenes, boost::test_tools::per_element());
  }
  
  // Nodes
  {
    std::vector<gltf::Node> expected{
      {
        gltf::Node({}, {}, {}, {}, {},
                   glm::quat(0.570, 0.169, 0.755, -0.272),
                   {},
                   glm::vec3(4.076, 5.903, -1.005),
                   {},
                   "Light"),
          gltf::Node({}, {}, {}, {}, {},
                     glm::quat(0.780, 0.483, 0.336, -0.208),
                     {},
                     glm::vec3(7.358, 4.95, 6.925),
                     {},
                     "Camera"),
          gltf::Node({}, {}, {}, {}, {0},
                     {},
                     {},
                     {},
                     {},
                     "Torus")
          }};
    
    BOOST_TEST(expected == asset._nodes, boost::test_tools::per_element());
  }

  // Meshes
  {
    std::vector<gltf::Mesh> expected{
      {
        gltf::Mesh({{gltf::Primitive({{"NORMAL", 1}, {"POSITION", 0}, {"TEXCOORD_0", 2}}, {3}, {0}, GL_TRIANGLES, {})}},
                   {},
                   "Torus")
          }};
    
    BOOST_TEST(expected == asset._meshes, boost::test_tools::per_element());
  }
}

BOOST_AUTO_TEST_SUITE_END()
