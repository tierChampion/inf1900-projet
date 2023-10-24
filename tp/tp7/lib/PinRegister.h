#pragma once
#define F_CPU 8000000UL
#include <avr/io.h>
typedef volatile uint8_t *Register;

struct Pin
{
    Pin(Register mode_, Register port_, uint8_t pin_);
    Register mode;
    Register port;
    uint8_t pin;
};