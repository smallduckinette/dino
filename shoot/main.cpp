#include <iostream>
#include <boost/program_options.hpp>

#include "graphicsystem.h"
#include "physicsystem.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    std::string shaderDir;
    
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Displays help")
      ("verbose,v", "Verbose logging")
      ("shaders", po::value<std::string>(&shaderDir)->default_value("."), "Shader directory");
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return 0;
    }
    
    GraphicSystem graphicSystem(shaderDir);
    PhysicSystem physicSystem;
    
    // Add ground
    
    
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
      
      graphicSystem.display();
    }
  }
  catch(const std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}
