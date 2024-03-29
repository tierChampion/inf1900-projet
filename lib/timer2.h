#ifndef TIMER_2_H
#define TIMER_2_H

#include "timer_commons.h"

class Timer2
{
public:
    Timer2();

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
    TimerWaveMode _waveMode;
    TimerInterrupt _interrupt;
    TimerPrescalar _prescalar;
};

#endif
