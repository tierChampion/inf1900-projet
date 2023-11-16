#ifndef BUTTON_H
#define BUTTON_H

#include "pins.h"
#include "general_interrupt.h"

class Button
{
public:
    Button(GeneralInterrupt interrupt, bool activeHigh);

    void update();
    bool getIsPressed() const;
    bool getHasChanged();
    bool isButtonPressed() const;
    void setSenseControl(SenseControl control);
    void enable();
    void disable();
    void clearButtonEvents();

private:
    ReadPin _pin;
    GeneralInterrupt _interrupt;
    bool _isActiveHigh;
    volatile bool _isPressed;
    volatile bool _hasChanged;
};

#endif
