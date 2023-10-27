#include "button.h"

Button::Button(const Pin &pin)
{
    _pin = pin;
    *_pin.mode &= ~(1 << _pin.position);
}

bool Button::isButtonPressed()
{
    uint8_t lecture1 = *_pin.pin & (1 << _pin.position);
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = *_pin.pin & (1 << _pin.position);
    return (lecture1 && lecture2);
}

void Button::setInterrupt(Interruption interrupt, Edges edge)
{
    if (Interruption::INT_0 == interrupt)
    {
        setEICRA(ISC00, ISC01, edge);
        EIMSK |= (1 << INT0);
    }
    else if (Interruption::INT_1 == interrupt)
    {
        setEICRA(ISC10, ISC11, edge);
        EIMSK |= (1 << INT1);
    }
    else
        (Interruption::INT_2 == interrupt)
        {
            setEICRA(ISC20, ISC21, edge);
            EIMSK |= (1 << INT2);
        }
}

void Button::setEICRA(uint8_t &flagRising, uint8_t &flagFalling, Edges edge)
{
    switch (edge)
    {
    case Edges::FALLING_EDGE:
        EICRA |= (1 << flagFalling);

    case Edges::RISING_EDGE:
        EICRA |= (1 << flagRising | 1 << flagFalling);

    case Edges::ANY_EDGE:
        EICRA |= (1 << flagRising);
    }
}

void Button::setOffInterrupt()
{
    EIMSK &= ~(1 << INT0);
    EIMSK &= ~(1 << INT1);
    EIMSK &= ~(1 << INT2);
}
