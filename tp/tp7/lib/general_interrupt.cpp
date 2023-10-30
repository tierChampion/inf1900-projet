#include "general_interrupt.h"

#include <avr/interrupt.h>

GeneralInterrupt::GeneralInterrupt(const GeneralInterruptType type)
    : _type(type)
{
    debugRequiredPin();
    _control = SenseControl::LOW_LEVEL;
    setFlags();
}

void GeneralInterrupt::enable()
{
    cli();
    EIMSK |= (1 << _maskFlag);
    sei();
}

void GeneralInterrupt::disable()
{
    cli();
    EIMSK &= ~(1 << _maskFlag);
    sei();
}

void GeneralInterrupt::clear()
{
    cli();
    EIFR &= ~(1 << _interruptFlag);
    sei();
}

void GeneralInterrupt::setFlags()
{
    switch (_type)
    {
    case GeneralInterruptType::INT_0:
        _maskFlag = INT0;
        _interruptFlag = INTF0;
        break;
    case GeneralInterruptType::INT_1:
        _maskFlag = INT1;
        _interruptFlag = INTF1;
        break;
    case GeneralInterruptType::INT_2:
        _maskFlag = INT2;
        _interruptFlag = INTF2;
        break;
    }
}

void GeneralInterrupt::setSenseControl(SenseControl control)
{
    cli();
    _control = control;
    applySenseControl();
    sei();
}

void GeneralInterrupt::applySenseControl()
{
    switch (_control)
    {
    case SenseControl::LOW_LEVEL:
        EICRA &= ~(1 << _controlFlag1 | 1 << _controlFlag2);
        break;
    case SenseControl::ANY_EDGE:
        EICRA &= ~(1 << _controlFlag2);
        EICRA |= (1 << _controlFlag1);
        break;
    case SenseControl::FALLING_EDGE:
        EICRA &= ~(1 << _controlFlag1);
        EICRA |= (1 << _controlFlag2);
        break;
    case SenseControl::RISING_EDGE:
        EICRA |= (1 << _controlFlag1 | 1 << _controlFlag2);
        break;
    }
}

ReadPin GeneralInterrupt::getRequiredPin() const
{
    switch (_type)
    {
    case GeneralInterruptType::INT_0:
        return ReadPin(Port::D, PD2);
        break;
    case GeneralInterruptType::INT_1:
        return ReadPin(Port::D, PD3);
        break;
    default:
        return ReadPin(Port::B, PB2);
        break;
    }
}

void GeneralInterrupt::debugRequiredPin() const
{
    switch (_type)
    {
    case GeneralInterruptType::INT_0:
        PRINT("Required Pin for INT0: D2\n");
        break;
    case GeneralInterruptType::INT_1:
        PRINT("Required Pin for INT1: D3\n");
        break;
    case GeneralInterruptType::INT_2:
        PRINT("Required Pin for INT2: B2\n");
        break;
    }
}