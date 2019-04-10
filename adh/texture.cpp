#include "texture.h"

#include <memory>
#include <SDL2/SDL_image.h>


adh::Texture::Texture(const std::string & name,
                      const std::string & filename):
  _name(name)
{
  glGenTextures(1, &_textureId);

  try
  {
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
      imageData(IMG_Load(filename.c_str()),
                SDL_FreeSurface);
    
    GLenum mode = (imageData->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 mode,
                 imageData->w,
                 imageData->h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 imageData->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  catch(const std::exception &)
  {
    glDeleteTextures(1, &_textureId);
    throw;
  }
}

adh::Texture::~Texture()
{
  glDeleteTextures(1, &_textureId);
}

void adh::Texture::bind()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureId);  
}
