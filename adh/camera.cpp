#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>


adh::Camera::Camera(float fov,
                    float ratio,
                    float minDistance,
                    float maxDistance,
                    const std::string & name):
  Node(name),
  _projection(glm::perspective(fov, ratio, minDistance, maxDistance)),
  _view(glm::lookAt(glm::vec3(0.0, 0.0, -1.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 1.0, 0.0)))
{
}

void adh::Camera::draw(Context & context) const
{
  glm::mat4 previousProjection = context._projection;
  glm::mat4 previousView = context._view;
  
  context._projection = _projection;
  context._view = _view;
  
  Node::draw(context);
  
  context._projection = previousProjection;
  context._view = previousView;
}

void adh::Camera::setViewMatrix(const glm::mat4 & view)  
{
  _view = view;
}
