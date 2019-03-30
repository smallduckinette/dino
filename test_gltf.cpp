#include <boost/test/unit_test.hpp>

#include <fstream>
#include <json/json.h>

#include "buffer.h"
#include "parser.h"

BOOST_AUTO_TEST_SUITE(Gltf)

BOOST_AUTO_TEST_CASE(testLoadAccessors)
{
  std::ifstream str("cube.gltf");
  Json::Value doc;
  str >> doc;
  
  auto accessors = gltf::loadAccessors(doc);
  BOOST_CHECK_EQUAL(4, accessors.size());
}

BOOST_AUTO_TEST_SUITE_END()
