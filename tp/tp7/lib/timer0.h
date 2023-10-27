
#ifndef TIMER_0_H
#define TIMER_0_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer_commons.h"

class Timer0
{
public:

    Timer0(TimerParameters parameters);

    void start();
    void stop();
    bool isRunning() const;

    void setCounterValue(uint8_t value);
    void setCompareValue(TimerCompare compare, uint8_t value);

    void setWaveMode(TimerWaveMode mode);
    void setCompareMode(TimerCompare compare, TimerCompareMode mode);
    void setInterrupt(TimerInterrupt interrupt);
    void setPrescalar(TimerPrescalar prescalar);

private:

    void applyInterrupt(TimerInterrupt interrupt);
    void applyPrescalar(TimerPrescalar prescalar);

private:

    bool _isTicking;
    Register _counter;
    Register _compareA;
    Register _compareB;
    TimerParameters _params;
    TimerWaveMode _waveMode;
    TimerInterrupt _runningInterrupt;
    TimerPrescalar _runningPrescalar;
};

#endif