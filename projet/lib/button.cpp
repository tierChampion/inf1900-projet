#include "button.h"

#include <util/delay.h>

const uint8_t DEBOUNCE_DELAY = 10;

Button::Button(GeneralInterrupt interrupt, bool activeHigh)
    : _interrupt(interrupt), _isActiveHigh(activeHigh), _isPressed(false), _hasChanged(false)
{
    _pin = _interrupt.getRequiredPin();
}

void Button::update()
{
    bool newPressed = isButtonPressed();
    _hasChanged = (newPressed != _isPressed);
    _isPressed = newPressed;
    clearButtonEvents();
}

bool Button::getIsPressed() const
{
    return _isPressed;
}

bool Button::getHasChanged() 
{
    bool temp = _hasChanged;
    _hasChanged = false;
    return temp;
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
