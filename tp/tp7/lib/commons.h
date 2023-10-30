#ifndef PIN_REGISTER_H
#define PIN_REGISTER_H

#include <avr/io.h>
#include "debug.h"

typedef volatile uint8_t *Register;
typedef volatile uint16_t *DoubleRegister;

typedef uint8_t Flag;
enum class Port
{
    A,
    B,
    C,
    D
};
struct Pin
{
    Pin(Port port_, uint8_t position_, bool mode_)
    {
        position = position_;
        switch (port_)
        {
        case Port::A:
            port = &PORTA;
            mode = &DDRA;
            pin = &PINA;
            break;
        case Port::B:
            port = &PORTB;
            mode = &DDRB;
            pin = &PINB;
            ;
            break;
        case Port::C:
            port = &PORTC;
            mode = &DDRC;
            pin = &PINC;
            break;
        default:
            port = &PORTD;
            mode = &DDRD;
            pin = &PIND;
            break;
        }
        if (mode_)
            *mode |= (1 << position_);
        else
            *mode &= ~(1 << position_);
    }

    Register mode;
    Register pin;
    Register port;
    uint8_t position;
};

#endif