#ifndef WHEEL_H
#define WHEEL_H

#include "PinRegister.h"
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
    Wheel(Pin directionPin = Pin{&DDRB, &PORTB, &PINB, PB5}, Side side = Side::RIGHT);
    ~Wheel();
    void setSpeed(Direction direction, float speed);
    Pin getDirPin() const;
    void setDirectionPin(Pin directionPin);

private:
    struct Pin _directionPin;
    Side _side;
};
#endif
