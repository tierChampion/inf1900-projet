#ifndef TIMER_1_H
#define TIMER_1_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer_commons.h"

class Timer1
{
public:

    Timer1(TimerControls parameters);

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
    DoubleRegister _counter;
    DoubleRegister _compareA;
    DoubleRegister _compareB;
    TimerControls _controls;
    TimerSettings _settings;
};

#endif