#ifndef WHEEL_H
#define WHEEL_H

#include "commons.h"
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
    Wheel(Pin directionPin, Side side);
    ~Wheel();
    void setSpeed(Direction direction, float speed);
    Pin getDirPin() const;
    void setDirectionPin(Pin directionPin);

private:
    struct Pin _directionPin;
    Side _side;
};
#endif
