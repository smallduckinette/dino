#include "material.h"

#include "texture.h"
#include "shader.h"

adh::Material::Material(const std::string & name,
                        const std::shared_ptr<Shader> & shader,
                        const std::shared_ptr<Texture> & texture):
  Node(name),
  _shader(shader),
  _texture(texture)
{
}

void adh::Material::draw(Context & context) const
{
  _shader->use();
  _shader->setMatrix("model", context._model);
  _shader->setMatrix("view", context._view);
  _shader->setMatrix("projection", context._projection);
  
  Node::draw(context);
}
