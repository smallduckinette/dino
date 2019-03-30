#include <vector>
#include <memory>

#include <json/json.h>
#include <GL/glew.h>


namespace gltf
{
  class Accessor;

  class Primitive
  {
  public:
    Primitive(const Json::Value & doc,
              const std::vector<std::shared_ptr<Accessor> > & accessors);
        
    ~Primitive();
    
    Primitive(const Primitive & other) = delete;
    Primitive & operator=(const Primitive & other) = delete;
    
    void draw() const;
    
  private:
    void cleanup();

    std::string _name;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    GLuint _elements;
    
    GLsizei _count;
    GLenum _type;
  };
  
  class Mesh
  {
  public:
    Mesh(const Json::Value & doc,
         const std::vector<std::shared_ptr<Accessor> > & accessors);
    
    void draw() const;
    
  private:
    std::string _name;
    std::vector<std::shared_ptr<Primitive> > _primitives;
  };
  
  class Node
  {
  public:
    Node(const Json::Value & doc,
         const std::vector<std::shared_ptr<Mesh> > & meshes);

  private:
    std::string _name;
    std::shared_ptr<Mesh> _mesh;
  };
  
  class Scene
  {
  public:
    Scene(const Json::Value & doc,
          const std::vector<std::shared_ptr<Node> > & nodes);
    
  private:
    std::vector<std::shared_ptr<Node> > _nodes;
  };
  
  std::vector<std::shared_ptr<Mesh> > parse(std::istream & str);
}