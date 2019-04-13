#include "transform.h"


adh::Transform::Transform():
  _matrix(1.0f)
{
}

void adh::Transform::draw(Context & context) const
{
  glm::mat4 previousModel = context._model;
  context._model *= _matrix;
  Node::draw(context);
  context._model = previousModel;
}

void adh::Transform::setMatrix(const glm::mat4 & matrix)
{
  _matrix = matrix;
}
