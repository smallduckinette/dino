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

#include "shader.h"
#include "parser.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Displays help")
      ("verbose,v", "Verbose logging")
      ("model,m", po::value<std::string>(), "Model to load");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return 0;
    }
    
    if(!vm.count("model"))
      throw std::runtime_error("No model selected");
    std::string model = vm["model"].as<std::string>();
    
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

    const int SCR_WIDTH = 512;
    const int SCR_HEIGHT = 512;
    
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
        
    glewExperimental = GL_TRUE;
    glewInit();
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    glEnable(GL_MULTISAMPLE);
    
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Version: " << version << std::endl;
    
    std::ifstream str(model);
    auto meshes = gltf::parse(str);
    
    std::ifstream vertex("shader.vert");
    std::ifstream fragment("shader.frag");
    Shader shader(vertex, fragment);
    
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
      glClear(GL_COLOR_BUFFER_BIT);
      
      shader.use();

      glm::mat4 model(1.0f);
      glm::mat4 view(1.0f);
      glm::mat4 projection(1.0f);
      model = glm::rotate(model, SDL_GetTicks() / 10000.0f, glm::vec3(0.5f, 1.0f, 0.0f));
      view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
      projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      
      shader.setMatrix("model", model);
      shader.setMatrix("view", view);
      shader.setMatrix("projection", projection);
      
      for(auto && mesh : meshes)
      {
        mesh->draw();
      }
      
      SDL_GL_SwapWindow(window.get());
    }
  }
  catch(const std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}
