#include "wheel.h"
Wheel::Wheel(Pin directionPin, Side side) : _directionPin(directionPin), _side(side)
{
    TimerControls params;
    params._controlA = &TCCR0A;
    params._controlB = &TCCR0B;
    params._interruptMask = &TIMSK0;
    _timerPWM = Timer0(params);
    _timerPWM.setCounterValue(0);
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
    *_directionPin.mode |= (1 << _directionPin.position);
    _timerPWM.start();
    PRINT("Creation of a Wheel object done\n");
}

Wheel::~Wheel()
{
    _timerPWM.stop();
    PRINT("Destruction of a Wheel object done\n");
}

void Wheel::setSpeed(Direction direction, float speed)
{
    switch (direction)
    {

    case Direction::BACKWARD:
        *_directionPin.port |= (1 << _directionPin.position);
        break;
    case Direction::FORWARD:
        *_directionPin.port &= ~(1 << _directionPin.position);
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

Pin Wheel::getDirPin() const
{
    return _directionPin;
}

void Wheel::setDirPin(Pin directionPin)
{
    *_directionPin.mode |= (1 << _directionPin.position);
    _directionPin = directionPin;
}
