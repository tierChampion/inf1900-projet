#pragma once
#define F_CPU 8000000UL
#include <avr/io.h>
typedef volatile uint8_t *Register;

struct Pin
{
    Pin(Register mode_, Register port_, Register pin_, uint8_t position);
    Register mode;
    Register port;
    Register pin;
    uint8_t position;
};