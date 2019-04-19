#ifndef __ADH_COLOR_H__
#define __ADH_COLOR_H__

namespace adh
{
  class Shader;
  
  class Color
  {
  public:
    virtual ~Color() = default;
    
    virtual void bind(Shader & shader) = 0;
    
  private:
  };
}

#endif
