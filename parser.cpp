#include "parser.h"

#include <boost/log/trivial.hpp>

#include "buffer.h"


gltf::Primitive::Primitive(const Json::Value & doc,
                           const std::vector<std::shared_ptr<Accessor> > & accessors):
  _name(doc.get("name", "").asString())
{
  BOOST_LOG_TRIVIAL(debug) << "Loading primitive " << _name;
  
  try
  {
    glGenVertexArrays(1, &_vertexArray);
    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_elements);

    glBindVertexArray(_vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    
    BOOST_LOG_TRIVIAL(debug) << "Binding elements...";
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements);
    auto indicesAccessor = accessors.at(doc.get("indices", "").asUInt());
    BOOST_LOG_TRIVIAL(debug) << *indicesAccessor;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indicesAccessor->getSize(),
                 indicesAccessor->getData(),
                 GL_STATIC_DRAW);
    
  }
  catch(const std::exception &)
  {
    cleanup();
    
    throw;
  }
}

gltf::Primitive::~Primitive()
{
  cleanup();
}

void gltf::Primitive::cleanup()
{
  glDeleteBuffers(1, &_elements);
  glDeleteBuffers(1, &_vertexBuffer);  
  glDeleteVertexArrays(1, &_vertexArray);
}

gltf::Mesh::Mesh(const Json::Value & doc,
                 const std::vector<std::shared_ptr<Accessor> > & accessors)
{
  for(auto && primitive : doc["primitives"])
  {
    _primitives.push_back(std::make_shared<Primitive>(primitive, accessors));
  }
}

std::vector<std::shared_ptr<gltf::Scene> > gltf::parse(std::istream & str)
{
  Json::Value doc;
  str >> doc;

  auto accessors = gltf::loadAccessors(doc);
  
  std::vector<std::shared_ptr<Mesh> > meshes;
  for(auto && mesh : doc["meshes"])
  {
    meshes.push_back(std::make_shared<Mesh>(mesh, accessors));
  }
  
  return {};
}
