#ifndef GENERAL_INTERRUPT_H
#define GENERAL_INTERRUPT_H

#include "pins.h"

enum class GeneralInterruptType
{
    INT_0,
    INT_1,
    INT_2
};

enum class SenseControl
{
    LOW_LEVEL,
    FALLING_EDGE,
    RISING_EDGE,
    ANY_EDGE
};

class GeneralInterrupt
{
public:
    GeneralInterrupt(const GeneralInterruptType type);

    void setSenseControl(SenseControl control);
    void enable();
    void disable();
    void clear();
    ReadPin getRequiredPin() const;

private:
    void setFlags();
    void applySenseControl();
    void debugRequiredPin() const;

private:
    GeneralInterruptType _type;
    Flag _controlFlag1;
    Flag _controlFlag2;
    Flag _maskFlag;
    Flag _interruptFlag;
    SenseControl _control;
};

#endif
