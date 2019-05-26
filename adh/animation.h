#ifndef __ADH_ANIMATION_H__
#define __ADH_ANIMATION_H__

#include <string>
#include <memory>
#include <vector>

namespace adh
{
  class Channel;
  class Clock;
  
  class Animation
  {
  public:
    Animation(const std::string & name,
              const std::shared_ptr<Clock> & clock);
    
    void addChannel(std::unique_ptr<Channel> channel);
    
    void start();
    void stop();
    void rewind();
    void setLoop(bool loop);
    void setSpeed(float speed);
    
    void update();
    
  private:
    std::string _name;
    std::shared_ptr<Clock> _clock;
    
    std::vector<std::unique_ptr<Channel> > _channels;
    
    bool _run;
    bool _loop;
    float _speed;
    float _maxTimestamp;
    
    float _simulationTimestamp;
    float _previousTimestamp;
  };
}

#endif
