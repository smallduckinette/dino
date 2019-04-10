#include "material.h"


adh::Material::Material(const std::string & name,
                        const std::shared_ptr<Texture> & texture):
  Node(name),
  _texture(texture)
{
}

void adh::Material::draw() const
{
  Node::draw();
}
