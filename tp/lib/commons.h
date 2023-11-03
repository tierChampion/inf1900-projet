#ifndef PIN_REGISTER_H
#define PIN_REGISTER_H

#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

typedef volatile uint8_t *Register;
typedef uint8_t Flag;

inline void configurableDelayMS(uint8_t count25)
{
    for (uint8_t i = 0; i < count25; i++)
    {
        _delay_ms(25);
    }
}

#endif
