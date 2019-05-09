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
  Sint16 view_x = SDL_JoystickGetAxis(_joystick.get(), 0);
  Sint16 view_y = SDL_JoystickGetAxis(_joystick.get(), 1);
  
  _viewSignal.emit(glm::vec2(normalize(view_x), normalize(view_y)));
}

Signal<glm::vec2> & Controller::onViewChange()
{
  return _viewSignal;
}

Signal<glm::vec2> & Controller::onPositionChange()
{
  return _positionSignal;
}

float Controller::normalize(Sint16 value) const
{
  if(std::abs(value) > 600)
  {
    return float(value - 600) / 32768.0;
  }
  else
    return 0.0f;
}
