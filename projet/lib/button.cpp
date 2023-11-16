#include "button.h"

#include <util/delay.h>

const uint8_t DEBOUNCE_DELAY = 10;

Button::Button(GeneralInterrupt interrupt, bool activeHigh)
    : _interrupt(interrupt), _isActiveHigh(activeHigh)
{
    _pin = _interrupt.getRequiredPin();
}

void Button::update()
{
    _isPressed = isButtonPressed();
    clearButtonEvents();
}

bool Button::getIsPressed()
{
    return _isPressed;
}

bool Button::isButtonPressed() const
{
    uint8_t lecture1 = _pin.read();
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = _pin.read();

    return (static_cast<bool>(lecture2) == _isActiveHigh) && (lecture1 == lecture2);
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
