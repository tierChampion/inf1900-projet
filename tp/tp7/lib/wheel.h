#ifndef WHEEL_H
#define WHEEL_H

#include "pins.h"
#include <avr/interrupt.h>

enum class Side
{
    RIGHT,
    LEFT
};

enum class Direction
{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};

class Wheel
{
public:
    Wheel(WritePin directionPin, Side side);
    ~Wheel();
    void setSpeed(Direction direction, float speed);
    WritePin getDirPin() const;
    void setDirectionPin(WritePin directionPin);

private:
    WritePin _directionPin;
    Side _side;
};
#endif
