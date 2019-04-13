#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

adh::Camera::Camera(float fov,
                    float ratio,
                    float minDistance,
                    float maxDistance,
                    const std::string & name):
  Node(name),
  _projection(glm::perspective(fov, ratio, minDistance, maxDistance))
{
}

void adh::Camera::draw(Context & context) const
{
  glm::mat4 previousProjection = context._projection;
  context._projection = _projection;
  Node::draw(context);
  context._projection = previousProjection;
}
