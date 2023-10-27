
#define F_CPU 8000000UL
#include <avr/io.h>
#include "debug.h"
#ifndef PIN_REGISTER_H
#define PIN_REGISTER_H
typedef volatile uint8_t *Register;

struct Pin
{
    Register mode;
    Register port;
    Register pin;
    uint8_t position;
};
#endif