#define F_CPU 8000000UL
#ifndef BUTTON_H
#define BUTTON_H
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PinRegister.h"
#include "general_interrupt.h"

class Button
{
public:
    Button(GeneralInterrupt interrupt, bool activeHigh);

    bool isButtonPressed() const;
    void setSenseControl(SenseControl control);
    void enable();
    void disable();
    void clearButtonEvents();

private:
    Pin _pin;
    bool _isActiveHigh;
    GeneralInterrupt _interrupt;
};
#endif
