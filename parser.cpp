#include "parser.h"

#include <numeric>
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
    
    BOOST_LOG_TRIVIAL(debug) << "Loading buffers...";
    
    std::vector<std::shared_ptr<Accessor> > selectedAccessors;
    selectedAccessors.push_back(accessors.at(doc["attributes"].get("POSITION", "").asUInt()));
    selectedAccessors.push_back(accessors.at(doc["attributes"].get("NORMAL", "").asUInt()));
    selectedAccessors.push_back(accessors.at(doc["attributes"].get("TEXCOORD_0", "").asUInt()));
    
    for(auto && accessor : selectedAccessors)
    {
      BOOST_LOG_TRIVIAL(debug) << *accessor;
    }
    
    size_t expectedCount = selectedAccessors.at(0)->getCount();
    for(auto && accessor : selectedAccessors)
    {
      if(accessor->getCount() != expectedCount)
        throw std::runtime_error("Inconsistent accessor count");
    }
    
    size_t totalSize = std::accumulate(selectedAccessors.begin(),
                                       selectedAccessors.end(),
                                       0,
                                       [](auto && acc, auto && accessor)
                                       {
                                         return acc + accessor->getSize();
                                       });
    size_t totalStride = std::accumulate(selectedAccessors.begin(),
                                         selectedAccessors.end(),
                                         0,
                                         [](auto && acc, auto && accessor)
                                         {
                                           return acc +
                                           accessor->getTypeSize() *
                                           accessor->getComponentSize();
                                         });
    
    std::vector<char> data;
    data.reserve(totalSize);
    
    // Stride the data
    for(size_t index = 0; index < expectedCount; ++index)
    {
      for(auto && accessor : selectedAccessors)
      {
        size_t stride = accessor->getTypeSize() * accessor->getComponentSize();
        std::copy(accessor->getData() + index * stride,
                  accessor->getData() + (index + 1) * stride,
                  std::back_inserter(data));
      }
    }

    BOOST_LOG_TRIVIAL(debug) << "Copying " << data.size() << " bytes of data out of expected " << totalSize;
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
    
    size_t index = 0;
    size_t offset = 0;
    for(auto && accessor : selectedAccessors)
    {
      BOOST_LOG_TRIVIAL(debug) << "glVertexAttribPointer("
                               << index << ", "
                               << accessor->getTypeSize() << ", "
                               << accessor->getComponentType() << ", "
                               << GL_FALSE << ", "
                               << totalStride << ", "
                               << offset << ")";
      
      glVertexAttribPointer(index++,
                            accessor->getTypeSize(),
                            accessor->getComponentType(),
                            GL_FALSE,
                            totalStride,
                            (void *)(offset));
      offset += accessor->getTypeSize() * accessor->getComponentSize();
    }
    
    BOOST_LOG_TRIVIAL(debug) << "Binding elements...";
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements);
    auto indicesAccessor = accessors.at(doc.get("indices", "").asUInt());
    _count = indicesAccessor->getCount();
    _type = indicesAccessor->getComponentType();

    BOOST_LOG_TRIVIAL(debug) << *indicesAccessor;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indicesAccessor->getSize(),
                 indicesAccessor->getData(),
                 GL_STATIC_DRAW);
    
    // Cleanup
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
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

void gltf::Primitive::draw() const
{
  glBindVertexArray(_vertexArray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements);
  glDrawElements(GL_TRIANGLES, _count, _type, 0);
}

void gltf::Primitive::cleanup()
{
  BOOST_LOG_TRIVIAL(debug) << "Cleaning";
  
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

void gltf::Mesh::draw() const
{
  for(auto && primitive : _primitives)
  {
    primitive->draw();
  }
}

std::vector<std::shared_ptr<gltf::Mesh> > gltf::parse(std::istream & str)
{
  Json::Value doc;
  str >> doc;

  auto accessors = gltf::loadAccessors(doc);
  
  std::vector<std::shared_ptr<Mesh> > meshes;
  for(auto && mesh : doc["meshes"])
  {
    meshes.push_back(std::make_shared<Mesh>(mesh, accessors));
  }
  
  return meshes;
}
