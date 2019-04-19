#include "plaincolor.h"

#include "shader.h"

adh::PlainColor::PlainColor(const std::string & reference,
                            const glm::vec4 & color):
  _reference(reference),
  _color(color)
{
}

void adh::PlainColor::bind(Shader & shader)
{
  shader.setVector(_reference, _color);
}
