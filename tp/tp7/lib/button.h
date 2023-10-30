#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pins.h"
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
    ReadPin _pin;
    GeneralInterrupt _interrupt;
    bool _isActiveHigh;
};
#endif
