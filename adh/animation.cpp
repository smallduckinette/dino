#include "animation.h"

#include "channel.h"
#include "clock.h"

adh::Animation::Animation(const std::string & name,
                          const std::shared_ptr<Clock> & clock):
  _name(name),
  _clock(clock),
  _run(false),
  _loop(false),
  _speed(1.0),
  _maxTimestamp(0.0)
{
}

void adh::Animation::addChannel(std::unique_ptr<Channel> channel)
{
  _channels.push_back(std::move(channel));
  _maxTimestamp = std::max(_maxTimestamp, _channels.back()->getMaxTimestamp());
}

void adh::Animation::start()
{
  _run = true;
  _simulationTimestamp = 0;
  _previousTimestamp = _clock->getTimestamp();
}

void adh::Animation::stop()
{
  _run = false;
}

void adh::Animation::rewind()
{
  _simulationTimestamp = 0;
}

void adh::Animation::setLoop(bool loop)
{
  _loop = loop;
}

void adh::Animation::setSpeed(float speed)
{
  _speed = speed;
}

void adh::Animation::update()
{
  float currentTimestamp = _clock->getTimestamp();

  if(_run)
  {
    float delta = currentTimestamp - _previousTimestamp;
    _simulationTimestamp += delta;
    while(_loop && _simulationTimestamp > _maxTimestamp)
    {
      _simulationTimestamp -= _maxTimestamp;
    }
    for(auto && channel : _channels)
    {
      channel->update(_simulationTimestamp);
    }
  }

  _previousTimestamp = currentTimestamp;
}
