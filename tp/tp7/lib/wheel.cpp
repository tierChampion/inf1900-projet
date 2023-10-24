#include "wheel.h"
#include <math.h>
Wheel::Wheel(Pin directionPin, Side side) : _directionPin(directionPin), _side(side)
{
    DDRB |= (1 << PB3 | 1 << PB4);
    TCNT0 = 0;
    TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B |= (1 << CS01);
    _speed = 0;
    *_directionPin.mode |= (1 << _directionPin.position);
}

void Wheel::setSpeed(int8_t speed)
{
    _speed = speed;
    if (speed >= 0)
        *_directionPin.port &= ~(1 << _directionPin.position);
    else
        *_directionPin.port |= (1 << _directionPin.position);
    switch (_side)
    {
    case Side::RIGHT:
        OCR0A = (uint8_t)((255 * fabs(speed)) / 100);
        break;
    case Side::LEFT:
        OCR0B = (uint8_t)((255 * fabs(speed)) / 100);
        break;
    default:
        break;
    }
}

int8_t Wheel::getSpeed() const
{
    return _speed;
}

void Wheel::setDirectionPin(Pin directionPin)
{
    _directionPin = directionPin;
}

Pin::Pin(Register mode_, Register port_, Register pin_, uint8_t position_)
{
    mode = mode_;
    port = port_;
    position = position_;
    pin = pin_;
}
