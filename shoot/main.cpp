#include <iostream>
#include <boost/program_options.hpp>

#include "entity/entityfactory.h"
#include "graphicsystem.h"
#include "physicsystem.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
  try
  {
    std::string shaderDir;
    std::string nodesFile;
    
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Displays help")
      ("verbose,v", "Verbose logging")
      ("shaders", po::value<std::string>(&shaderDir)->default_value("."), "Shader directory")
      ("nodes", po::value<std::string>(&nodesFile)->default_value("nodes.json"), "Nodes file");
    
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
    
    physicSystem.onMove().connect(std::bind(&GraphicSystem::move,
                                            std::ref(graphicSystem),
                                            std::placeholders::_1,
                                            std::placeholders::_2));
    
    EntityFactory entityFactory(nodesFile);
    entityFactory.registerSystem("graphics", &graphicSystem);
    entityFactory.registerSystem("physics", &physicSystem);
    
    entityFactory.addEntity("ball");
    
    auto floor = entityFactory.addEntity("floor");
    physicSystem.move(floor, glm::vec3(0, -5, 0));    
    
    bool running = true;
    while(running)
    {
      auto t1 = std::chrono::steady_clock::now();
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT)
        {
          running = false;
        }
      }
      
      graphicSystem.display();
      auto t2 = std::chrono::steady_clock::now();
      std::chrono::duration<float> d = (t2 - t1);
      physicSystem.run(d.count());
    }
  }
  catch(const std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}
