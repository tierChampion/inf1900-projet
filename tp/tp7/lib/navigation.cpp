#include "navigation.h"

Navigation::Navigation(Pin dirLeftPin, Pin dirRightPin) : _leftWheel(Wheel(dirLeftPin, Side::LEFT)), _rightWheel(Wheel(dirRightPin, Side::RIGHT))
{
    PRINT("Creation of a Navigation object done.\n");
}

Navigation::~Navigation()
{
    PRINT("Destruction of a Navigation object done.\n");
}

void Navigation::turn(Direction direction, float speed)
{
    switch (direction)
    {
    case Direction::LEFT:
        _leftWheel.setSpeed(Direction::FORWARD, 0);
        _rightWheel.setSpeed(Direction::FORWARD, speed);
        break;
    case Direction::RIGHT:
        _leftWheel.setSpeed(Direction::FORWARD, speed);
        _rightWheel.setSpeed(Direction::FORWARD, 0);
    default:
        break;
    }
}

void Navigation::move(Direction direction, float speed)
{
    _leftWheel.setSpeed(direction, speed);
    _rightWheel.setSpeed(direction, speed);
}

void Navigation::stop()
{
    move(Direction::FORWARD, 0);
}

void Navigation::setDirPin(Side side, Pin directionPin)
{
    switch (side)
    {
    case Side::LEFT:
        _leftWheel.setDirPin(directionPin);
        break;
    case Side::RIGHT:
        _rightWheel.setDirPin(directionPin);
        break;
    default:
        break;
    }
}

Pin Navigation::getDirPin(Side side) const
{
    switch (side)
    {
    case Side::RIGHT:
        return _rightWheel.getDirPin();

    default:
        return _leftWheel.getDirPin();
    }
}
