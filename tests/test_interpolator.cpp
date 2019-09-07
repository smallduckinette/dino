#include <boost/test/unit_test.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "adh/interpolator.h"

namespace glm
{
  std::ostream & operator<<(std::ostream & str, const vec3 & v)
  {
    str << "{" << v[0] << ", " << v[1] << ", " << v[2] << "}";
    return str;
  }
}

BOOST_AUTO_TEST_SUITE(Adh)

BOOST_AUTO_TEST_CASE(testLinearInterpolator)
{
  adh::LinearInterpolator<float> i({{2, 3}, {6, 8}});
  BOOST_CHECK_CLOSE(i.get(0), 3, 0.01);
  BOOST_CHECK_CLOSE(i.get(1), 3, 0.01);
  BOOST_CHECK_CLOSE(i.get(2), 3, 0.01);
  BOOST_CHECK_CLOSE(i.get(3), 4.25, 0.01);
  BOOST_CHECK_CLOSE(i.get(4), 5.5, 0.01);
  BOOST_CHECK_CLOSE(i.get(5), 6.75, 0.01);
  BOOST_CHECK_CLOSE(i.get(6), 8, 0.01);
  BOOST_CHECK_CLOSE(i.get(7), 8, 0.01);

  BOOST_CHECK_CLOSE(i.getMax(), 6, 0.01);
}

BOOST_AUTO_TEST_CASE(testLinearInterpolatorVec3)
{
  adh::LinearInterpolator<glm::vec3> i
    (
     {
       {1, glm::vec3(0, 1, 0)},
       {3, glm::vec3(2, 1, 0)},
       {5, glm::vec3(4, 3, 0)}
     }
     );
  
  BOOST_CHECK_EQUAL(i.get(0), glm::vec3(0, 1, 0));
  BOOST_CHECK_EQUAL(i.get(1), glm::vec3(0, 1, 0));
  BOOST_CHECK_EQUAL(i.get(2), glm::vec3(1, 1, 0));
  BOOST_CHECK_EQUAL(i.get(3), glm::vec3(2, 1, 0));
  BOOST_CHECK_EQUAL(i.get(4), glm::vec3(3, 2, 0));
  BOOST_CHECK_EQUAL(i.get(5), glm::vec3(4, 3, 0));
  BOOST_CHECK_EQUAL(i.get(6), glm::vec3(4, 3, 0));

  BOOST_CHECK_CLOSE(i.getMax(), 5, 0.01);
}

BOOST_AUTO_TEST_CASE(testCubicSplineInterpolator)
{
  adh::CubicSplineInterpolator<float> i({{2, {3, 0, 0}}, {6, {8, 0, 0}}});
  BOOST_CHECK_CLOSE(i.get(0), 3, 0.01);
  BOOST_CHECK_CLOSE(i.get(1), 3, 0.01);
  BOOST_CHECK_CLOSE(i.get(2), 3, 0.01);
  BOOST_CHECK_CLOSE(i.get(3), 3.78125, 0.01);
  BOOST_CHECK_CLOSE(i.get(4), 5.5, 0.01);
  BOOST_CHECK_CLOSE(i.get(5), 7.21875, 0.01);
  BOOST_CHECK_CLOSE(i.get(6), 8, 0.01);
  BOOST_CHECK_CLOSE(i.get(7), 8, 0.01);
  
  BOOST_CHECK_CLOSE(i.getMax(), 6, 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
