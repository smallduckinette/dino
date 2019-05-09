#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>


adh::Camera::Camera(float fov,
                    float ratio,
                    float minDistance,
                    float maxDistance,
                    const std::string & name):
  Node(name),
  _projection(glm::perspective(fov, ratio, minDistance, maxDistance)),
  _heading(0),
  _pitch(0)
{
}

void adh::Camera::draw(Context & context) const
{
  glm::mat4 previousProjection = context._projection;
  context._projection = _projection;
  
  glm::vec3 position(0.0f, 0.0f, -1.0f);
  glm::vec4 front(0.0f, 0.0f, 1.0f, 0.0f);
  glm::vec4 up(0.0f, 1.0f, 0.0f, 0.0f);
  glm::mat4 heading_matrix = glm::rotate(glm::mat4(1.0f), _heading, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 pitch_matrix = glm::rotate(glm::mat4(1.0f), _pitch, glm::vec3(1.0f, 0.0f, 0.0f));
  
  context._view = glm::lookAt(position,
                              position + glm::vec3(heading_matrix * pitch_matrix * front),
                              glm::vec3(heading_matrix * pitch_matrix * up));
  
  Node::draw(context);
  context._projection = previousProjection;
}

void adh::Camera::updateView(float delta_heading, float delta_pitch)
{
  _heading += delta_heading;
  _pitch += delta_pitch;
  if(_pitch > M_PI / 2)
    _pitch = M_PI / 2;
  else if(_pitch < -M_PI / 2)
    _pitch = -M_PI / 2;
}
