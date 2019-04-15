#include "material.h"

#include "texture.h"
#include "shader.h"

adh::Material::Material(const std::string & name,
                        const std::shared_ptr<Shader> & shader,
                        const std::shared_ptr<Texture> & diffuseTexture,
                        const std::shared_ptr<Texture> & normalTexture,
                        const std::shared_ptr<Texture> & metalRoughnessTexture):
  Node(name),
  _shader(shader),
  _diffuseTexture(diffuseTexture),
  _normalTexture(normalTexture),
  _metalRoughnessTexture(metalRoughnessTexture)
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
  _shader->setInteger("diffuseMap", 0);
  _shader->setInteger("normalMap", 1);
  _shader->setInteger("metalroughnessMap", 2);

  _diffuseTexture->bind(GL_TEXTURE0);
  _normalTexture->bind(GL_TEXTURE1);
  _metalRoughnessTexture->bind(GL_TEXTURE2);
  
  Node::draw(context);
}
