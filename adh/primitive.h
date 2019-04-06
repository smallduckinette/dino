#ifndef __ADH_PRIMITIVE_H__
#define __ADH_PRIMITIVE_H__

#include <GL/glew.h>

#include "node.h"

namespace adh
{
  class Primitive : public Node
  {
  public:
    Primitive();
    ~Primitive();

    /// Call bind before setting arrays of data
    void bind();

    /// Optionally call unbind after setting arrays of data
    void unbind();

    /// Set vertex data
    void setVertexData(const char * data, size_t size);

    /// Describe vertex data - Call it for each type of data in the vertex data
    void describeVertexData(size_t index,
                            size_t typeSize,
                            GLenum componentType,
                            size_t stride,
                            size_t offset);
    
    /// Set index data
    void setIndexData(const char * data, size_t size);
    
    /// Describe index data
    void describeIndexData(size_t count, GLenum type);
    
  private:
    void onDraw() const override;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    GLuint _elements;    

    GLsizei _count;
    GLenum _type;
  };
}

#endif
