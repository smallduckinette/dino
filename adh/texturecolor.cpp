#include "texturecolor.h"

#include "shader.h"
#include "texture.h"

adh::TextureColor::TextureColor(const std::shared_ptr<Texture> & texture,
                                GLenum textureType,
                                const std::string & textureReference):
  _texture(texture),
  _textureType(textureType),
  _textureReference(textureReference)
{
}

void adh::TextureColor::bind(Shader & shader)
{
  shader.setInteger(_textureReference, _textureType - GL_TEXTURE0);
  _texture->bind(_textureType);
}
