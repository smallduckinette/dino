#include <boost/test/unit_test.hpp>

#include <fstream>
#include <json/json.h>

#include "buffer.h"

BOOST_AUTO_TEST_SUITE(Gltf)

BOOST_AUTO_TEST_CASE(testLoadFile)
{
  std::ifstream str("cube.gltf");
  Json::Value doc;
  str >> doc;
  
  gltf::Buffer buffer(doc);
}

BOOST_AUTO_TEST_SUITE_END()
