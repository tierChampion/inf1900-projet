#include "wheel.h"

Wheel::Wheel()
{
}

Wheel::Wheel(WritePin directionPin, Side side, Timer0 *timerWheels)
    : _directionPin(directionPin), _side(side), _timerWheels(timerWheels)
{
}

void Wheel::setSpeed(Orientation orientation, uint8_t speed)
{
    switch (orientation)
    {
    case Orientation::BACKWARD:
        _directionPin.set();
        break;
    case Orientation::FORWARD:
        _directionPin.clear();
        break;
    }

    switch (_side)
    {
    case Side::RIGHT:
        _timerWheels->setCompareValue(TimerCompare::B, speed);
        break;
    case Side::LEFT:
        _timerWheels->setCompareValue(TimerCompare::A, speed);
        break;
    }
}
