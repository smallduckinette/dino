#ifndef __ADH_PRIMITIVE_H__
#define __ADH_PRIMITIVE_H__

#include <optional>
#include <GL/glew.h>

#include "node.h"

namespace adh
{
  class Shader;
  class Texture;
  
  class Primitive : public Node
  {
  public:
    Primitive(GLenum mode);
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

    /// Set geometry data
    size_t setDataBuffer(const char * data,
                         size_t size,
                         GLint componentCount,
                         GLenum componentType,
                         bool normalize,
                         size_t stride,
                         size_t offset);
      
    /// Set index data
    void setIndicesBuffer(const char * data,
                          size_t size,
                          size_t count,
                          GLenum type);
    
    /// Set texture
    void setTexture(const std::shared_ptr<Texture> & texture, GLenum textureIndex);
    
    /// Set color
    void setColor(const std::string & label, const glm::vec4 & color);
    
    /// Set shader
    void setShader(const std::shared_ptr<Shader> & shader);
    
  private:
    void draw(Context & context) const override;
    
    GLuint _vertexArray;
    std::optional<GLuint> _elements;
    std::vector<GLuint> _buffers;
    std::vector<std::pair<GLenum, std::shared_ptr<Texture> > > _textures;
    std::vector<std::pair<std::string, glm::vec4> > _colors;
    
    GLenum _mode;
    
    GLsizei _count;
    GLenum _type;
    
    std::shared_ptr<Shader> _shader;
  };
}

#endif
