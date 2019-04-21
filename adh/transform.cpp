#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

adh::Transform::Transform():
  _matrix(1.0f),
  _translate(1.0f),
  _rotate(1.0f),
  _scale(1.0f)
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

void adh::Transform::setTranslate(const glm::vec3 & translate)
{
  _translate = glm::translate(glm::mat4(1.0), translate);
  recomputeMatrix();
}

void adh::Transform::setRotate(const glm::quat & rotate)
{
  _rotate = glm::mat4_cast(rotate);
  recomputeMatrix();
}

void adh::Transform::setScale(const glm::vec3 & scale)
{
  _scale = glm::scale(glm::mat4(1.0), scale);
  recomputeMatrix();
}

void adh::Transform::recomputeMatrix()
{
  _matrix = _translate * _rotate * _scale;
}
