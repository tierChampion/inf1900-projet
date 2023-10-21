#include "navigation.h"

Navigation::Navigation(Pin dirRightPin = Pin(&DDRB, &PORTB, PB5), Pin dirLeftPin = Pin(&DDRB, &PORTB, PB6))
{
    _leftWheel = Wheel(dirLeftPin, Side::LEFT);
    _rightWheel = Wheel(dirRightPin, Side::RIGHT);
}

void Navigation::turn(Side direction, int8_t speed)
{
    switch (direction)
    {
    case Side::LEFT:
        _leftWheel.setSpeed(0);
        _rightWheel.setSpeed((int8_t)abs(speed));
        break;
    case Side::RIGHT:
        _leftWheel.setSpeed((int8_t)abs(speed));
        _rightWheel.setSpeed(0);
    default:
        break;
    }
}

void Navigation::go(int8_t speed)
{

    _leftWheel.setSpeed(speed);
    _rightWheel.setSpeed(speed);
}
