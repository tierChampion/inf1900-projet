#include "PinRegister.h"
#include <avr/interrupt.h>
#ifndef WHEEL_H
#define WHEEL_H
enum Side
{
    RIGHT_WHEEL,
    LEFT_WHEEL
};
enum Direction
{
    FORWARD,
    BACKWARD,
    RIGHT_DIR,
    LEFT_DIR
};
class Wheel
{
public:
    Wheel(Pin directionPin = Pin(&DDRB, &PORTB, &PINB, PB5), Side side = Side::RIGHT_WHEEL);
    ~Wheel();
    void setSpeed(Direction direction, float speed);
    Pin getDirPin() const;
    void setDirectionPin(Pin directionPin);

private:
    struct Pin _directionPin;
    uint8_t _side;
};
#endif
