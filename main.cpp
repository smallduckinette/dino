#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "adh/primitive.h"
#include "adh/camera.h"
#include "adh/shader.h"
#include "adh/transform.h"
#include "gltf/nodecache.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    std::string shaderDir;
    std::string model;
    
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Displays help")
      ("verbose,v", "Verbose logging")
      ("vsync", "enable vsync")
      ("shaders", po::value<std::string>(&shaderDir)->default_value("."), "Shader directory")
      ("model,m", po::value<std::string>(&model), "Model to load");
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return 0;
    }
    
    if(model.empty())
      throw std::runtime_error("No model selected");

    if(vm.count("verbose"))
    {
      boost::log::core::get()->set_filter
        (boost::log::trivial::severity >= boost::log::trivial::debug);
    }
    else
    {
      boost::log::core::get()->set_filter
        (boost::log::trivial::severity >= boost::log::trivial::info);
    }
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
      throw std::runtime_error(std::string("Failed to init SDL : ") + SDL_GetError());

    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>
      window(SDL_CreateWindow("Dino",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCR_WIDTH,
                              SCR_HEIGHT,
                              SDL_WINDOW_OPENGL),
             SDL_DestroyWindow);
    
    std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)>
      glContext(SDL_GL_CreateContext(window.get()),
                SDL_GL_DeleteContext);

    if(vm.count("vsync"))
    {
      SDL_GL_SetSwapInterval(1);
    }
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST); 
    
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    
    BOOST_LOG_TRIVIAL(info) << "Renderer: " << renderer << std::endl;
    BOOST_LOG_TRIVIAL(info) << "Version: " << version << std::endl;
    
    gltf::NodeCache nodeCache(shaderDir, model);
    auto primitive = nodeCache.getMesh(0);
    auto transform = std::make_shared<adh::Transform>();
    auto camera = std::make_shared<adh::Camera>(glm::radians(45.0f),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                0.1f,
                                                100.0f);
    transform->addChild(primitive);
    camera->addChild(transform);
    
    auto t1 = std::chrono::system_clock::now();
    
    bool running = true;
    while(running)
    {
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT)
        {
          running = false;
        }
      }
      
      glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      transform->setMatrix(glm::rotate(glm::mat4(1.0f), SDL_GetTicks() / 1000.0f, glm::vec3(0.5f, 1.0f, 0.0f)));
      
      adh::Context context;
      camera->draw(context);
      
      SDL_GL_SwapWindow(window.get());

      auto t2 = std::chrono::system_clock::now();
      std::chrono::duration<double> d = (t2 - t1);
      BOOST_LOG_TRIVIAL(debug) << 1 / d.count() << std::endl;
      t1 = t2;
    }
  }
  catch(const std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}
