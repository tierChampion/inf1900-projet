#include "wheel.h"
Wheel::Wheel(Pin directionPin, Side side) : _directionPin(directionPin), _side(side)
{
    TCNT0 = 0;
    TCCR0A |= (1 << WGM00);
    TCCR0B |= (1 << CS01);
    switch (_side)
    {
    case Side::LEFT_WHEEL:
        DDRB |= (1 << PB3);
        TCCR0A |= (1 << COM0A1);
        break;

    default:
        DDRB |= (1 << PB4);
        TCCR0A = (1 << COM0B1);
        break;
    }
    *_directionPin.mode |= (1 << _directionPin.position);
    char string[] = "Creation of a Wheel object done";
    PRINT(string);
}

Wheel::~Wheel()
{
    TCCR0A &= ~(1 << WGM00);
    TCCR0B &= ~(1 << CS01);
    switch (_side)
    {
    case Side::LEFT_WHEEL:
        TCCR0A &= ~(1 << COM0A1);
        break;

    default:
        TCCR0A &= ~(1 << COM0B1);
        break;
    }
    char string[] = "Destruction of a Wheel object done";
    PRINT(string);
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
        char string[] = "Can Only use FORWARD OR BACKWARD to set a wheel speed .\n Use Navigation.turn to turn";
        PRINT(string);
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
