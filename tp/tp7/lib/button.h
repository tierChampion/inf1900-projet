#define F_CPU 8000000UL
#ifndef BUTTON_H
#define BUTTON_H
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PinRegister.h"

typedef volatile uint8_t *Register;

enum class Edges
{
    FALLING_EDGE,
    RISING_EDGE,
    ANY_EDGE
};

enum class Interruption
{
    INT_0,
    INT_1,
    INT_2
};

class Button
{
public:
    Button(const Pin &pin);

    bool isButtonPressed();
    void setEICRA(uint8_t flagRising, uint8_t flagFalling, Edges edge);
    void setInterrupt(Interruption interrupt, Edges edge);
    void setOffInterrupt();

private:
    Pin _pin;
    static const uint8_t DEBOUNCE_DELAY = 20;
};
#endif
