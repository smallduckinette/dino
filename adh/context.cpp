#include "context.h"

#include <glm/gtc/matrix_transform.hpp>

adh::Context::Context():
  _model(1.0f),
  _view(glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f),
                    glm::vec3(0.0f, 0.0f, -2.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f))),
  _projection(1.0f),
  _camPos(glm::vec3(0.0f, 0.0f, -3.0f)),
  _lightPosition(0.0f, 0.0f, -3.0f),
  _lightColor(1.0f, 1.0f, 1.0f)
{
}
