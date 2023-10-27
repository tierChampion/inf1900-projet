#ifndef PIN_REGISTER_H
#define PIN_REGISTER_H

#include <avr/io.h>
#include "debug.h"

typedef volatile uint8_t *Register;
typedef volatile uint16_t* DoubleRegister;

struct Pin
{
    Register mode;
    Register port;
    Register pin;
    uint8_t position;

    Pin();
    Pin(Register modeP, Register portP, Register pinP, uint8_t positionP);
};

#endif