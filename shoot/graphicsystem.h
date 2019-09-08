#ifndef __SHOOT_GRAPHICSYSTEM_H__
#define __SHOOT_GRAPHICSYSTEM_H__

#include <memory>
#include <SDL2/SDL.h>
#include "entity/system.h"
#include "adh/camera.h"
#include "adh/transform.h"

class GraphicSystem : public System
{
public:
  GraphicSystem(const std::string & shaderDirectory);
  ~GraphicSystem();
  
  void display();
  
  void add(EntityId entityId, const Json::Value & doc) override;
  
private:
  std::string _shaderDirectory;
  SDL_Window * _window;
  SDL_GLContext _glContext;
  
  std::shared_ptr<adh::Camera> _camera;
  std::map<EntityId, std::shared_ptr<adh::Transform> > _entities;
};

#endif
