#include "graphicsystem.h"

#include <GL/glew.h>
#include "core/json_utils.h"
#include "adh/animation.h"
#include "gltf/builder.h"

GraphicSystem::GraphicSystem(const std::filesystem::path & shaderDirectory):
  _shaderDirectory(shaderDirectory)
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    throw std::runtime_error(std::string("Failed to init SDL : ") + SDL_GetError());  
  
  const int SCR_WIDTH = 1024;
  const int SCR_HEIGHT = 768;
  
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
  
  _window = SDL_CreateWindow("Dino",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCR_WIDTH,
                             SCR_HEIGHT,
                             SDL_WINDOW_OPENGL);
  
  _glContext = SDL_GL_CreateContext(_window);
  
  SDL_GL_SetSwapInterval(1);
  
  glewExperimental = GL_TRUE;
  glewInit();
  
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST); 
  _camera = std::make_shared<adh::Camera>(glm::radians(45.0f),
                                          (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                          0.1f,
                                          100.0f);
}

GraphicSystem::~GraphicSystem()
{
  SDL_GL_DeleteContext(_glContext);
  SDL_DestroyWindow(_window);
}

void GraphicSystem::display()
{
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  adh::Context context;
  _camera->draw(context);
  
  SDL_GL_SwapWindow(_window);  
}

void GraphicSystem::init(const std::filesystem::path & rootDirectory)
{
  _modelDirectory = rootDirectory;
}

void GraphicSystem::add(EntityId entityId, const Json::Value & doc)
{
  std::string model;
  core::get(doc, "model", model);
  
  gltf::Builder builder(nullptr, _shaderDirectory, _modelDirectory / model);
  std::vector<std::unique_ptr<adh::Animation> > animations;
  
  auto transform = std::make_shared<adh::Transform>();
  transform->addChild(builder.build(animations));
  
  _entities.insert({entityId, transform});
}
