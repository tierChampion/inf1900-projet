#include "avr_useful_lib.h"
typedef volatile uint8_t *Register;
enum Side
{
    RIGHT,
    LEFT
};
struct Pin
{
    Pin(Register mode_, Register port_, uint8_t pin_);
    Register mode;
    Register port;
    uint8_t pin;
};
class Wheel
{
public:
    Wheel(Pin directionPin = Pin(&DDRB, &PORTB, PB5), Side side = Side::RIGHT);
    void setSpeed(int8_t speed);
    int8_t getSpeed();
    void setDirectionPin(Pin directionPin);

private:
    struct Pin _directionPin;
    uint8_t _side;
    int8_t _speed;
};