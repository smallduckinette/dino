#ifndef __ADH_CHANNEL_H__
#define __ADH_CHANNEL_H__

#include <functional>

#include "interpolator.h"
#include "transform.h"

namespace adh
{
  class Channel
  {
  public:
    virtual ~Channel() = default;
    
    virtual void update(float timestamp) = 0;
    virtual float getMaxTimestamp() const = 0;
  };

  template<typename T>
  class InterpolatedChannel : public Channel
  {
  public:
    InterpolatedChannel(std::unique_ptr<Interpolator<T> > interpolator,
                        const std::shared_ptr<Transform> & transform,
                        std::function<void(Transform * transform, const T &)> f):
      _interpolator(std::move(interpolator)),
      _transform(transform),
      _f(f)
    {
    }
    
    void update(float timestamp) override
    {
      _f(_transform.get(), _interpolator.get(timestamp));
    }

    float getMaxTimestamp() const override
    {
      return _interpolator->getMax();
    }
    
  private:
    std::unique_ptr<Interpolator<T> > _interpolator;
    std::shared_ptr<Transform> _transform;
    std::function<void(const T &)> _f;
  };
}

#endif
