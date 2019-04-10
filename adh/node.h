#ifndef __ADH_NODE_H__
#define __ADH_NODE_H__

#include <memory>
#include <string>
#include <vector>

namespace adh
{
  class Node
  {
  public:
    Node();
    Node(const std::string & name);
    virtual ~Node()
    {
    }
    
    virtual void draw() const;
    
    void addChild(const std::shared_ptr<Node> & child);
    
  private:
    std::string _name;
    std::vector<std::shared_ptr<Node> > _children;
  };
}

#endif
