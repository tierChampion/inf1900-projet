#include "PinRegister.h"
#include <avr/interrupt.h>
#ifndef WHEEL_H
#define WHEEL_H
enum Side
{
    RIGHT,
    LEFT
};
enum Direction
{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};
class Wheel
{
public:
    Wheel(Pin directionPin = Pin(&DDRB, &PORTB, &PINB, PB5), Side side = Side::RIGHT);
    ~Wheel();
    void setSpeed(Direction direction, uint8_t speed);
    uint8_t getSpeed() const;
    Pin getDirPin() const;
    void setDirectionPin(Pin directionPin);

private:
    struct Pin _directionPin;
    uint8_t _side;
    // pair<Direction, uint8_t> _speed;
};
#endif
