#include "wheel.h"

Wheel::Wheel()
{
}

Wheel::Wheel(WritePin directionPin, Side side, Timer0 *timerPwm)
    : _directionPin(directionPin), _side(side), _timerPWM(timerPwm)
{

    _timerPWM->start();
}

Wheel::~Wheel()
{
    _timerPWM->stop();
}

void Wheel::setSpeed(Direction direction, double speed)
{
    switch (direction)
    {

    case Direction::BACKWARD:
        _directionPin.set();
        break;
    case Direction::FORWARD:
        _directionPin.clear();
        break;
    default:
        break;
    }
    switch (_side)
    {
    case Side::RIGHT:
        _timerPWM->setCompareValue(TimerCompare::B, (uint8_t)(255 * speed / 100));
        PRINT(OCR0B);
        break;
    case Side::LEFT:
        _timerPWM->setCompareValue(TimerCompare::A, (uint8_t)(255 * speed / 100));
        PRINT(OCR0A);
        break;
    default:
        break;
    }
}
