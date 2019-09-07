#ifndef __ADH_INTERPOLATOR_H__
#define __ADH_INTERPOLATOR_H__

#include <map>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace adh
{
  template<typename T>
  T interpolate(const T & left, const T & right, float n)
  {
    return (1 - n) * left + n * right;
  }
  
  inline glm::quat interpolate(const glm::quat & left, const glm::quat & right, float n)
  {
    return glm::slerp(left, right, n);
  }  
  
  template<typename T>
  class Interpolator
  {
  public:
    virtual T get(float t) const = 0;
    virtual float getMax() const = 0;
  };
  
  template<typename T>
  class LinearInterpolator : public Interpolator<T>
  {
  public:
    LinearInterpolator(const std::map<float, T> & values):
      _values(values)
    {
    }
    
    T get(float t) const override
    {
      auto it = _values.lower_bound(t);
      if(it == _values.begin()) // We are before the beginning, return first value
      {
        return _values.begin()->second;
      }
      else if(it == _values.end()) // We are after the end, send last value
      {
        return std::prev(_values.end())->second;
      }
      else
      {
        auto t1 = std::prev(it);
        auto t2 = it;
        
        float n = (t - t1->first) / (t2->first - t1->first);
        return interpolate(t1->second, t2->second, n);
      }
    }

    float getMax() const override
    {
      if(_values.empty())
        return 0;
      else
        return _values.rbegin()->first;
    }
    
  private:
    std::map<float, T> _values;
  };

  template<typename T>
  class CubicSplineInterpolator : public Interpolator<T>
  {
  public:
    CubicSplineInterpolator(const std::map<float, std::tuple<T, T, T> > & values):
      _values(values)
    {
    }

    T get(float t) const override
    {
      auto it = _values.lower_bound(t);
      if(it == _values.begin()) // We are before the beginning, return first value
      {
        return std::get<0>(_values.begin()->second);
      }
      else if(it == _values.end()) // We are after the end, send last value
      {
        return std::get<0>(std::prev(_values.end())->second);
      }
      else
      {
        auto t1 = std::prev(it);
        auto t2 = it;
        
        float n = (t - t1->first) / (t2->first - t1->first);
        
        T vk = std::get<1>(t1->second);
        T bk = std::get<2>(t1->second);
        T vk1 = std::get<1>(t2->second);
        T ak1 = std::get<0>(t2->second);
        
        T p0 = vk;
        T m0 = (t2->first - t1->first) * bk;
        T p1 = vk1;
        T m1 = (t2->first - t1->first) * ak1;

        return
          (2 * n * n * n - 3 * n * n + 1) * p0 +
          (n * n * n - 2 * n * n + n) * m0 +
          (-2 * n * n * n + 3 * n * n) * p1 +
          (n * n * n - n * n) * m1;
      }
    }
    
    float getMax() const override
    {
      if(_values.empty())
        return 0;
      else
        return _values.rbegin()->first;
    }

  private:
    std::map<float, std::tuple<T, T, T> > _values;
  };
}

#endif
