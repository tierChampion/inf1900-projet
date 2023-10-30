#include "button.h"

const uint8_t DEBOUNCE_DELAY = 20;

Button::Button(GeneralInterrupt interrupt, bool activeHigh)
    : _interrupt(interrupt), _isActiveHigh(activeHigh)
{
    _pin = _interrupt.getRequiredPin();
}

bool Button::isButtonPressed() const
{
    uint8_t lecture1 = _pin.read();
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = _pin.read();

    return (lecture2 == _isActiveHigh) && (lecture1 == lecture2);
}

void Button::setSenseControl(SenseControl control)
{
    _interrupt.setSenseControl(control);
}

void Button::enable()
{
    _interrupt.enable();
}

void Button::disable()
{
    _interrupt.disable();
}

void Button::clearButtonEvents()
{
    _interrupt.clear();
}
