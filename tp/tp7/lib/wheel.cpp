#include "wheel.h"
Wheel::Wheel(Pin directionPin, Side side) : _directionPin(directionPin), _side(side)
{
    DDRB |= (1 << PB3 | 1 << PB4);
    TCNT0 = 0;
    TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B |= (1 << CS01);
    //_speed = 0;
    *_directionPin.mode |= (1 << _directionPin.position);
    PRINT("Creation of a Wheel object done");
}

Wheel::~Wheel()
{
    PRINT("Destruction of a Wheel object done");
}

void Wheel::setSpeed(Direction direction, uint8_t speed)
{
    //_speed = std::pair(direction, speed);
    switch (direction)
    {
    case Direction::BACKWARD:
        *_directionPin.port |= (1 << _directionPin.position);
        break;
    case Direction::FORWARD:
        *_directionPin.port &= ~(1 << _directionPin.position);
        break;
    default:
        PRINT(("Can Only use FORWARD OR BACKWARD to set a wheel speed .\n Use Navigation.turn to turn".c_str()));
    }
    switch (_side)
    {
    case Side::RIGHT_WHEEL:
        OCR0A = (uint8_t)(255 * speed / 100);
        break;
    case Side::LEFT_WHEEL:
        OCR0B = (uint8_t)(255 * speed / 100);
        break;
    default:
        break;
    }
}

uint8_t Wheel::getSpeed() const
{
    return 0; //_speed;
}

Pin Wheel::getDirPin() const
{
    return _directionPin;
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
