/*
#include "button.h"
#include <util/delay.h>
Pin::Pin(Register mode_, Register port_, Register pin_, uint8_t position)
{
    mode = mode_;
    port = port_;
    position = position_;
    pin = pin_;
}

Button::Button(Pin pin)
{
    position_ = position;
    *pin_.mode &= ~(1<<pin_.position);
}

bool Button::isButtonPressed()
{
    uint8_t lecture1 = *pin_.pin & (1 << pin_.position);
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = *pin_.pin & (1 << pin_.position);
    return (lecture1 && lecture2);
}

void Button::setOnInterrupt(Edges edge, Interruption interrupt)
{
    switch (interrupt)
    {
    case INT0:
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
    case INT1:
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
    case INT2:
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
            sei();
}
void Button::setOffInterrupt()
{
    EIMSK &= ~(1<< INT0)
    EIMSK &= ~(1<< INT1)
    EIMSK &= ~(1<<INT2)
    EIMSK &= ~(1 << INT1);
    EIMSK &= ~(1 << INT2);
}

*/
