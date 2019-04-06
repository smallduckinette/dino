#ifndef __ADH_NODE_H__
#define __ADH_NODE_H__

namespace adh
{
  class Node
  {
  public:
    virtual ~Node() = default;
    
    virtual void draw() const = 0;
    
  private:
    
  };
}

#endif
