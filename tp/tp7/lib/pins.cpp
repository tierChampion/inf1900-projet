#include "pins.h"

ReadPin::ReadPin(Port pinPort, uint8_t position)
{
    switch (pinPort)
    {
    case Port::A:
        _mode = &DDRA;
        _pin = &PINA;
        break;
    case Port::B:
        _mode = &DDRB;
        _pin = &PINB;
        break;
    case Port::C:
        _mode = &DDRC;
        _pin = &PINC;
        break;
    case Port::D:
        _mode = &DDRD;
        _pin = &PIND;
        break;
    }

    _position = position;
    *_mode &= ~(1 << _position);
}

uint8_t ReadPin::read()
{
    return (*_pin & (1 << _position));
}

WritePin::WritePin(Port pinPort, uint8_t position)
{

    switch (pinPort)
    {
    case Port::A:
        _mode = &DDRA;
        _port = &PORTA;
        break;
    case Port::B:
        _mode = &DDRB;
        _port = &PORTB;
        break;
    case Port::C:
        _mode = &DDRC;
        _port = &PORTC;
        break;
    case Port::D:
        _mode = &DDRD;
        _port = &PORTD;
        break;
    }

    _position = position;
    *_mode |= (1 << _position);
}

void WritePin::set()
{
    *_port |= (1 << _position);
}

void WritePin::clear()
{
    *_port &= ~(1 << _position);
}