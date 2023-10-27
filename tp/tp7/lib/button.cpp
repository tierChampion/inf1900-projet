#include "button.h"

Button::Button(const Pin &pin)
{
    _pin = pin;
    *_pin.mode &= ~(1 << _pin.position);
}

bool Button::isButtonPressed()
{
    // verifier que cest egal a 1 a la fin pour confirmer que c<est allume
    uint8_t lecture1 = *_pin.pin & (1 << _pin.position);
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = *_pin.pin & (1 << _pin.position);
    return (lecture1 && lecture2);
}

void Button::setOnInterrupt(Edges edge, Interruption interrupt)
{
    cli();
    switch (interrupt)
    {
    case Interruption::INT_0:
        switch (edge)
        {
        case FALLING_EDGE:
            EICRA |= (1 << ISC01);
            EIMSK |= (1 << INT0);
            break;
        case RISING_EDGE:
            EICRA |= (1 << ISC00);
            EIMSK |= (1 << INT0);
            break;
        case ANY_EDGE:
            EICRA |= (1 << ISC00) | (1 << ISC01);
            EIMSK |= (1 << INT0);
            break;
        default:
            break;
        }
        break;
    case Interruption::INT_1:
        switch (edge)
        {
        case FALLING_EDGE:
            EICRA |= (1 << ISC11);
            EIMSK |= (1 << INT1);
            break;
        case RISING_EDGE:
            EICRA |= (1 << ISC10) | (1 << ISC11);
            EIMSK |= (1 << INT1);
            break;
        case ANY_EDGE:
            EICRA |= (1 << ISC10);
            EIMSK |= (1 << INT1);
            break;
        }
        break;
    case Interruption::INT_2:
        switch (edge)
        {
        case FALLING_EDGE:
            EICRA |= (1 << ISC21);
            EIMSK |= (1 << INT2);
            break;
        case RISING_EDGE:
            EICRA |= (1 << ISC20) | (1 << ISC21);
            EIMSK |= (1 << INT2);
            break;
        case ANY_EDGE:
            EICRA |= (1 << ISC20);
            EIMSK |= (1 << INT2);

            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    sei();
}

void Button::setOffInterrupt()
{
    EIMSK &= ~(1 << INT0);
    EIMSK &= ~(1 << INT1);
    EIMSK &= ~(1 << INT2);
}
