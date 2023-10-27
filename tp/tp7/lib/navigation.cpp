#include "navigation.h"

Navigation::Navigation(Pin dirRightPin, Pin dirLeftPin)
{
    _leftWheel = Wheel(dirLeftPin, Side::LEFT_WHEEL);
    _rightWheel = Wheel(dirRightPin, Side::RIGHT_WHEEL);
    char string[] = "Creation of a Navigation object done";
    PRINT(string);
}

Navigation::~Navigation()
{
    char string[] = "Destruction of a Navigation object done";
    PRINT(string);
}

void Navigation::turn(Direction direction, uint8_t speed)
{
    switch (direction)
    {
    case Direction::LEFT_DIR:
        _leftWheel.setSpeed(Direction::FORWARD, 0);
        _rightWheel.setSpeed(Direction::FORWARD, speed);
        break;
    case Direction::RIGHT_DIR:
        _leftWheel.setSpeed(Direction::FORWARD, speed);
        _rightWheel.setSpeed(Direction::FORWARD, 0);
    default:
        break;
    }
}

void Navigation::move(Direction direction, uint8_t speed)
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
    case Side::LEFT_WHEEL:
        _leftWheel.setDirectionPin(directionPin);
        break;
    case Side::RIGHT_WHEEL:
        _rightWheel.setDirectionPin(directionPin);
        break;
    default:
        break;
    }
}

Pin Navigation::getDirPin(Side side) const
{
    switch (side)
    {
    case Side::RIGHT_WHEEL:
        return _rightWheel.getDirPin();

    default:
        return _leftWheel.getDirPin();
    }
}
