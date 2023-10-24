#include "navigation.h"

Navigation::Navigation(Pin dirRightPin, Pin dirLeftPin)
{
    _leftWheel = Wheel(dirLeftPin, Side::LEFT);
    _rightWheel = Wheel(dirRightPin, Side::RIGHT);
}

void Navigation::turn(Side direction, uint8_t speed)
{
    switch (direction)
    {
    case Side::LEFT:
        _leftWheel.setSpeed(0);
        _rightWheel.setSpeed((int8_t)speed);
        break;
    case Side::RIGHT:
        _leftWheel.setSpeed((int8_t)speed);
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

void Navigation::stop()
{
    go(0);
}

void Navigation::setDirPin(Side side, Pin directionPin)
{
    switch (side)
    {
    case Side::LEFT:
        _leftWheel.setDirectionPin(directionPin);
        break;
    case Side::RIGHT:
        _rightWheel.setDirectionPin(directionPin);
        break;
    default:
        break;
    }
}
