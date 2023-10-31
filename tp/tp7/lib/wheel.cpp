#include "wheel.h"

Wheel::Wheel(WritePin directionPin, Side side)
    : _directionPin(directionPin), _side(side), _timerPWM(Timer0())
{
    _timerPWM.setCounterValue(0);
    _timerPWM.setInterrupt(TimerInterrupt::NONE);
    _timerPWM.setPrescalar(TimerPrescalar::NO_PRESCALAR);
    _timerPWM.setWaveMode(TimerWaveMode::PWM_PHASE_CORRECT);

    switch (_side)
    {
    case Side::LEFT:

        _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::CLEAR);
        break;

    default:
        _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::CLEAR);
        break;
    }
    _timerPWM.start();
    PRINT("Wheel:Creation of a Wheel object done\n");
}

Wheel::~Wheel()
{
    _timerPWM.stop();
    PRINT("Wheel:Destruction of a Wheel object done\n");
}

void Wheel::setSpeed(Direction direction, float speed)
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
        PRINT("Wheel::setSpeed:Can Only use FORWARD OR BACKWARD to set a wheel speed .\n Use Navigation.turn to turn");
    }
    switch (_side)
    {
    case Side::RIGHT:
        _timerPWM.setCompareValue(TimerCompare::B, (uint8_t)(255 * speed / 100));

        break;
    case Side::LEFT:
        _timerPWM.setCompareValue(TimerCompare::A, (uint8_t)(255 * speed / 100));

        break;
    default:
        break;
    }
    PRINT("Wheel:Speed has been set\n");
}

WritePin Wheel::getDirPin() const
{
    return _directionPin;
}

void Wheel::setDirPin(WritePin directionPin)
{
    _directionPin = directionPin;
}
