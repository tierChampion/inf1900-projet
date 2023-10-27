#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "PinRegister.h"

enum class TimerWaveMode {
    NORMAL,
    CTC,
    PWM_PHASE_CORRECT
};

enum class TimerCompare {
    A,
    B
};

enum class TimerCompareMode {
    DISCONNECTED,
    TOGGLE,
    CLEAR
};

enum class TimerInterrupt {
    NONE,
    COMPARE_A,
    COMPARE_B,
    BOTH
};

enum class TimerPrescalar {
    STOPPED,
    NO_PRESCALAR,
    EIGHT,
    THIRTY_TWO,
    SIXTY_FOUR,
    ONE_TWENTY_EIGHT,
    TWO_FIFTY_SIX,
    THOUSAND_TWENTY_FOUR
};

struct TimerControls
{
    Register _controlA;
    Register _controlB;
    Register _interruptMask;
};

struct TimerSettings
{
    TimerWaveMode _waveMode;
    TimerInterrupt _interrupt;
    TimerPrescalar _prescalar;
};

#endif