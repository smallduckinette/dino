#include "material.h"

#include "texture.h"
#include "shader.h"
#include "color.h"

adh::Material::Material(const std::string & name,
                        const std::shared_ptr<Shader> & shader,
                        const std::vector<std::shared_ptr<Color> > & colors):
  Node(name),
  _shader(shader),
  _colors(colors)
{
}

void adh::Material::draw(Context & context) const
{
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
    color->bind(*_shader);
  }
  
  Node::draw(context);
}