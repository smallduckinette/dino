#include "primitive.h"



adh::Primitive::Primitive()
{
  glGenVertexArrays(1, &_vertexArray);
  glGenBuffers(1, &_vertexBuffer);
  glGenBuffers(1, &_elements);
}

adh::Primitive::~Primitive()
{
  glDeleteBuffers(1, &_elements);
  glDeleteBuffers(1, &_vertexBuffer);  
  glDeleteVertexArrays(1, &_vertexArray);
}

void adh::Primitive::bind()
{
  glBindVertexArray(_vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements);
}

void adh::Primitive::unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void adh::Primitive::setIndexData(const char * data, size_t size)
{
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void adh::Primitive::describeIndexData(size_t count, GLenum type)
{
  _count = count;
  _type = type;
}

void adh::Primitive::draw(Context & context) const
{
  glBindVertexArray(_vertexArray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements);
  glDrawElements(GL_TRIANGLES, _count, _type, 0);
  
  Node::draw(context);
}
