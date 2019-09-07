#ifndef __ENTITY_STRONGID_H__
#define __ENTITY_STRONGID_H__

#include <ostream>

template <typename D, typename T>
class StrongId
{
public:
  typedef D DiscriminantT;
  typedef T UnderlyingTypeT;
  
  StrongId()
  {
  }
      
  StrongId(const T & value):
    _value(value)
  {
  }

  StrongId(const StrongId<D, T> & other):
    _value(other._value)
  {
  }

  StrongId<D, T> & operator=(const StrongId<D, T> & other)
  {
    _value = other._value;
    return *this;
  }

  bool operator==(const StrongId<D, T> & other) const
  {
    return (_value == other._value);
  }

  bool operator!=(const StrongId<D, T> & other) const
  {
    return (_value != other._value);
  }

  bool operator<(const StrongId<D, T> & other) const
  {
    return (_value < other._value);
  }

  T getValue() const
  {
    return _value;
  }

private:
  T _value;
};

namespace std
{
  template<typename D>
  struct hash<StrongId<D, uint32_t> >
  {
    std::size_t operator()(const StrongId<D, uint32_t> & id) const
    {
      return id.getValue();
    }
  };
}

template<typename D, typename T>
std::ostream & operator<<(std::ostream & os, const StrongId<D, T> & id)
{
  return os << id.getValue();
}

template<typename T>
class StrongIdGenerator;

template <typename D, typename T>
class StrongIdGenerator<StrongId<D, T> >
{
public:
  StrongIdGenerator(const T & init = T()):
    _nextValue(init)
  {
  }
  
  StrongId<D, T> generate()
  {
    StrongId<D, T> value(_nextValue);
    _nextValue++;
    return value;
  }
  
private:
  T _nextValue;
};

#endif
