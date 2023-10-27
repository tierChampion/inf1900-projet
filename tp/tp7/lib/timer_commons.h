#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "PinRegister.h"

struct TimerParameters
{
    uint8_t _timerId;
    Register _controlA;
    Register _controlB;
    Register _controlC;
    Register _interruptMask;
};

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

class Timer
{
public:

    Timer(TimerParameters parameters);

    void start();
    void stop();
    bool isRunning() const;

    void setCounterValue(uint16_t value);
    void setCompareValue(TimerCompare compare, uint16_t value);

    void setWaveMode(TimerWaveMode mode);
    void setCompareMode(TimerCompare compare, TimerCompareMode mode);
    void setInterrupt(TimerInterrupt interrupt);
    void setPrescalar(TimerPrescalar prescalar);

private:

    void applyInterrupt(TimerInterrupt interrupt);
    void applyPrescalar(TimerPrescalar prescalar);

private:

    bool _isTicking;
    TimerParameters _params;
    TimerWaveMode _waveMode;
    TimerInterrupt _runningInterrupt;
    TimerPrescalar _runningPrescalar;
};

#endif