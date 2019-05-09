#ifndef __CHAPITRE7_SIGNAL_H__
#define __CHAPITRE7_SIGNAL_H__

#include <functional>
#include <vector>


template<typename ...ARGS>
class Signal
{
public:
  Signal() = default;
  Signal(const Signal &) = delete;
  Signal & operator=(const Signal &) = delete;
  
  void emit(ARGS... args)
  {
    for(auto && callback : _callbacks)
    {
      callback(args...);
    }
  }
  
  void connect(std::function<void(ARGS...)> f)
  {
    _callbacks.push_back(f);
  }

  template<typename C, typename F>
  void connect(C * obj, F f)
  {
    _callbacks.push_back([obj, f](ARGS... args){ (obj->*f)(args...); });
  }
  
private:
  std::vector<std::function<void(ARGS...)> > _callbacks;
};

#endif
