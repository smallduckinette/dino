#include "context.h"

#include <glm/gtc/matrix_transform.hpp>

adh::Context::Context():
  _model(1.0f),
  _view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f))),
  _projection(1.0f),
  _lightDirection(0.0f, 0.0f, 1.0f),
  _lightColor(1.0f, 1.0f, 1.0f)
{
}
