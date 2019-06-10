#ifndef __ADH_RTCLOCK_H__
#define __ADH_RTCLOCK_H__

#include <chrono>

#include "clock.h"

namespace adh
{
  class RtClock : public Clock
  {
  public:
    RtClock();
    
    float getTimestamp() const override;
    
  private:
    std::chrono::time_point<std::chrono::steady_clock> _dawnOfTimes;
  };
}

#endif
