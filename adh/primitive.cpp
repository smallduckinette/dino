#include "primitive.h"

#include "shader.h"
#include "texture.h"

#include <iostream>

adh::Primitive::Primitive(GLenum mode):
  _mode(mode)
{
  glGenVertexArrays(1, &_vertexArray);
}

adh::Primitive::~Primitive()
{
  for(auto && buffer : _buffers)
  {
    glDeleteBuffers(1, &buffer);
  }
  if(_elements)
  {
    glDeleteBuffers(1, &*_elements);
  }
  glDeleteVertexArrays(1, &_vertexArray);
}

void adh::Primitive::bind()
{
  glBindVertexArray(_vertexArray);
}

void adh::Primitive::unbind()
{
  glBindVertexArray(0);  
}

void adh::Primitive::setVertexData(const char * data, size_t size)
{
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void adh::Primitive::describeVertexData(size_t index,
                                        size_t typeSize,
                                        GLenum componentType,
                                        size_t stride,
                                        size_t offset)
{
  glVertexAttribPointer(index,
                        typeSize,
                        componentType,
                        GL_FALSE,
                        stride,
                        reinterpret_cast<void *>(offset));
  glEnableVertexAttribArray(index);  
}

size_t adh::Primitive::setDataBuffer(const char * data,
                                     size_t size,
                                     GLint componentCount,
                                     GLenum componentType,
                                     bool normalize,
                                     size_t stride,
                                     size_t offset)
{
  GLuint buffer;
  glGenBuffers(1, &buffer);
  _buffers.push_back(buffer);
  size_t index = _buffers.size() - 1;
  
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(index,
                        componentCount,
                        componentType,
                        normalize,
                        stride,
                        reinterpret_cast<void *>(offset));
  glEnableVertexAttribArray(index);  
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return index;
}

void adh::Primitive::setIndicesBuffer(const char * data,
                                      size_t size,
                                      size_t count,
                                      GLenum type)
{
  GLuint elements;
  glGenBuffers(1, &elements);
  _elements = elements;
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  _count = count;
  _type = type;
}

void adh::Primitive::setTexture(const std::shared_ptr<Texture> & texture, GLenum textureIndex)
{
  _textures.push_back({textureIndex, texture});
}

void adh::Primitive::setColor(const std::string & label, const glm::vec4 & color)
{
  _colors.push_back({label, color});
}

void adh::Primitive::setShader(const std::shared_ptr<Shader> & shader)
{
  _shader = shader;
}

void adh::Primitive::draw(Context & context) const
{
  // Bind geometry
  glBindVertexArray(_vertexArray);
  if(_elements)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_elements);
  }

  // Bind textures
  for(auto && texture : _textures)
  {
    texture.second->bind(texture.first);
    _shader->setInteger(texture.second->getName(), texture.first - GL_TEXTURE0);
  }
  
  // Setup shader
  _shader->use();
  _shader->setMatrix("modelMatrix", context._model);
  _shader->setMatrix("viewMatrix", context._view);
  _shader->setMatrix("projectionMatrix", context._projection);
  _shader->setMatrix("normalMatrix", glm::transpose(glm::inverse(context._model)));
  _shader->setVector("camPos", context._camPos);
  _shader->setVector("lightPosition", context._lightPosition);
  _shader->setVector("lightColor", context._lightColor);

  for(auto && color : _colors)
  {
    _shader->setVector(color.first, color.second);
  }
  
  glDrawElements(_mode, _count, _type, 0);
  
  Node::draw(context);
}
