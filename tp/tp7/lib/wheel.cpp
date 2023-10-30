#include "wheel.h"
Wheel::Wheel(WritePin directionPin, Side side) : _directionPin(directionPin), _side(side)
{
    TCNT0 = 0;
    TCCR0A |= (1 << WGM00);
    TCCR0B |= (1 << CS01);
    switch (_side)
    {
    case Side::LEFT:
        DDRB |= (1 << PB3);
        TCCR0A |= (1 << COM0A1);
        break;

    default:
        DDRB |= (1 << PB4);
        TCCR0A = (1 << COM0B1);
        break;
    }
    PRINT("Creation of a Wheel object done");
}

Wheel::~Wheel()
{
    TCCR0A &= ~(1 << WGM00);
    TCCR0B &= ~(1 << CS01);
    switch (_side)
    {
    case Side::LEFT:
        TCCR0A &= ~(1 << COM0A1);
        break;

    default:
        TCCR0A &= ~(1 << COM0B1);
        break;
    }
    PRINT("Destruction of a Wheel object done");
}

void Wheel::setSpeed(Direction direction, float speed)
{
    //_speed = std::pair(direction, speed);
    switch (direction)
    {
    case Direction::BACKWARD:
        _directionPin.set();
        break;
    case Direction::FORWARD:
        _directionPin.clear();
        break;
    default:
        PRINT("Can Only use FORWARD OR BACKWARD to set a wheel speed .\n Use Navigation.turn to turn");
    }
    switch (_side)
    {
    case Side::RIGHT:
        OCR0A = (uint8_t)(255 * speed / 100);
        break;
    case Side::LEFT:
        OCR0B = (uint8_t)(255 * speed / 100);
        break;
    default:
        break;
    }
}

WritePin Wheel::getDirPin() const
{
    return _directionPin;
}

void Wheel::setDirectionPin(WritePin directionPin)
{
    _directionPin = directionPin;
}
