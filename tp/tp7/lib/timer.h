#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

typedef volatile uint8_t *Register;

struct TimerParameters
{
    Register counter;
    Register compareA;
    Register compareB;
    Register controlA;
    Register controlB;
    Register controlC;
    Register interruptMask;
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
    SIXTY_FOUR,
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

    void setInterrupts(TimerInterrupt interrupts);
    void setPrescalar(TimerPrescalar prescalar);

private:

    void applyPrescalar(TimerPrescalar prescalar);

private:

    bool _isTicking;
    TimerParameters _params;
    TimerPrescalar _runningPrescalar;
};

#endif