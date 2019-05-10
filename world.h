#ifndef __DINO3_WORLD_H__
#define __DINO3_WORLD_H__

#include <memory>
#include <glm/glm.hpp>

namespace adh { class Camera; }

class World
{
public:
  World(const std::shared_ptr<adh::Camera> & came1ra);
  World(const World &) = delete;
  World & operator=(const World &) = delete;
  
  void onChangeView(const glm::vec2 & deltaLook);
  void onChangePosition(const glm::vec2 & deltaMove);
  
  void run(float deltaT);
  
private:
  std::shared_ptr<adh::Camera> _camera;
  
  glm::vec2 _deltaLook;
  glm::vec2 _look;

  glm::vec2 _deltaMove;

  glm::vec3 _position;

  float _maxSpeed;
  float _maxStrafe;
};

#endif
