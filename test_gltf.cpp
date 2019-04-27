#include <boost/test/unit_test.hpp>

#include <iostream>
#include "gltf/asset.h"

namespace std
{
  template<typename T>
  ostream & operator<<(ostream & str, const optional<T> & opt)
  {
    str << "{";
    if(opt)
      str << *opt;
    str << "}";
    return str;
  }
}

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
    BOOST_TEST(1 == asset._scenes.size());
    BOOST_CHECK_EQUAL("Scene", asset._scenes.at(0)._name);
    std::vector<size_t> expectedNodes{0, 1, 2};
    BOOST_TEST(expectedNodes == asset._scenes.at(0)._nodes, boost::test_tools::per_element());
  }
}

BOOST_AUTO_TEST_SUITE_END()
