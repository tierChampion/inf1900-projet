#include "wheel.h"

Wheel::Wheel()
{
}

Wheel::Wheel(WritePin directionPin, Side side, Timer0 *timerPwm)
    : _directionPin(directionPin), _side(side), _timerPWM(timerPwm)
{
}

void Wheel::setSpeed(Orientation orientation, float speed)
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
            _timerPWM->setCompareValue(TimerCompare::B, static_cast<uint8_t>(255 * speed / 100));
            PRINT(OCR0B);
            break;
        case Side::LEFT:
            _timerPWM->setCompareValue(TimerCompare::A, static_cast<uint8_t>(255 * speed / 100));
            PRINT(OCR0A);
            break;
    }
}
