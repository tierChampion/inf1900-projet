#ifndef COMMONS_H
#define COMMONS_H

#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

typedef volatile uint8_t *Register;
typedef uint8_t Flag;

const uint8_t MAXIMUM_8BIT = 255;
const float MAXIMUM_8BIT_F = 255.0f;
const float MAXIMUM_PERCENTAGE = 100.0f;

inline void configurableDelayMS(uint8_t count25)
{
    for (uint8_t i = 0; i < count25; i++)
    {
        _delay_ms(25);
    }
}

inline float toPercentage(uint8_t val)
{
    return (static_cast<float>(val) / MAXIMUM_8BIT_F) * MAXIMUM_PERCENTAGE;
}

inline uint8_t to8Bits(float val)
{
    return static_cast<uint8_t>(MAXIMUM_8BIT_F * val / MAXIMUM_PERCENTAGE);
}

inline uint8_t max(uint8_t a, uint8_t b)
{
    if (a > b)
        return a;
    return b;
}

inline uint8_t min(uint8_t a, uint8_t b)
{
    if (a < b)
        return a;
    return b;
}

inline uint8_t difference(uint8_t a, uint8_t b)
{
    if (a > b)
        return a - b;
    return b - a;
}

#endif
