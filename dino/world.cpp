#include "world.h"

#include <glm/gtc/matrix_transform.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "adh/camera.h"

World::World(const std::shared_ptr<adh::Camera> & camera):
  _camera(camera),
  _deltaLook(0, 0),
  _look(0, 0),
  _deltaMove(0, 0),
  _position(0.0, 0.0, -1.0),
  _maxSpeed(5),
  _maxStrafe(5)
{
}

void World::onChangeView(const glm::vec2 & deltaLook)
{
  _deltaLook = deltaLook;
}

void World::onChangePosition(const glm::vec2 & deltaMove)
{
  _deltaMove = -deltaMove;
}

void World::run(float deltaT)
{
  _look += _deltaLook * deltaT;
  _deltaLook = glm::vec2(0, 0);
  
  // Attitude
  glm::mat4 base(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), // Front
                 glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), // Right
                 glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), // Up
                 glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)); // Empty
  
  glm::mat4 heading_matrix = glm::rotate(glm::mat4(1.0f), -_look[0], glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 pitch_matrix = glm::rotate(glm::mat4(1.0f), -_look[1], glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 look = heading_matrix * pitch_matrix * base;
  
  // Position
  _position +=
    glm::vec3(look[0]) * _deltaMove[0] * deltaT * _maxSpeed + 
    glm::vec3(look[1]) * _deltaMove[1] * deltaT * _maxStrafe;
  
  _camera->setViewMatrix(glm::lookAt(_position,
                                     _position + glm::vec3(look[0]),
                                     glm::vec3(look[2])));
}
