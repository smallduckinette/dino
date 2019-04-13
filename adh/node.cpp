#include "node.h"

adh::Node::Node()
{
}

adh::Node::Node(const std::string & name):
  _name(name)
{
}

void adh::Node::draw(Context & context) const
{
  for(auto && child : _children)
  {
    child->draw(context);
  }
}

void adh::Node::addChild(const std::shared_ptr<Node> & child)
{
  _children.push_back(child);
}
