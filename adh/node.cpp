#include "node.h"

adh::Node::Node()
{
}

adh::Node::Node(const std::string & name):
  _name(name)
{
}

void adh::Node::draw() const
{
  onDraw();

  for(auto && child : _children)
  {
    child->draw();
  }
}

void adh::Node::addChild(const std::shared_ptr<Node> & child)
{
  _children.push_back(child);
}
