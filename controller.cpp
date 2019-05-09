#include "controller.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

Controller::Controller(int device_index):
  _joystick(SDL_JoystickOpen(device_index), &SDL_JoystickClose)
{
  if(!_joystick)
    throw std::runtime_error(std::string("Failed to create joystick: ") + SDL_GetError());
  
  BOOST_LOG_TRIVIAL(info) << "Loaded joystick " << SDL_JoystickName(_joystick.get());
}

void Controller::update()
{
  float view_x = SDL_JoystickGetAxis(_joystick.get(), 0);
  float view_y = SDL_JoystickGetAxis(_joystick.get(), 1);
  
  _viewSignal.emit(glm::vec2(view_x / 32768.0, view_y / 32768.0));
}

Signal<glm::vec2> & Controller::onViewChange()
{
  return _viewSignal;
}

Signal<glm::vec2> & Controller::onPositionChange()
{
  return _positionSignal;
}
