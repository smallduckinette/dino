#ifndef __DINO3_CONTROLLER_H__
#define __DINO3_CONTROLLER_H__

#include <memory>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "signal.h"

class Controller
{
public:
  Controller(int device_index);
  
  void update();
  
  Signal<glm::vec2> & onViewChange();
  Signal<glm::vec2> & onPositionChange();
  
private:
  std::unique_ptr<SDL_Joystick, decltype(&SDL_JoystickClose)> _joystick;
  
  Signal<glm::vec2> _viewSignal;
  Signal<glm::vec2> _positionSignal;
};

#endif
