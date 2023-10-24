#include "PinRegister.h"

#include <avr/interrupt.h>
enum Side
{
    RIGHT,
    LEFT
};

class Wheel
{
public:
    Wheel(Pin directionPin = Pin(&DDRB, &PORTB, PB5), Side side = Side::RIGHT);
    void setSpeed(int8_t speed);
    int8_t getSpeed() const;
    void setDirectionPin(Pin directionPin);

private:
    struct Pin _directionPin;
    uint8_t _side;
    int8_t _speed;
};
