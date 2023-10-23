#include "timer.h"

Timer::Timer(TimerParameters parameters)
    : _params(parameters)
{
    setPrescalar(TimerPrescalar::STOPPED);
    _isTicking = false;
}

void Timer::start()
{
    applyPrescalar(_runningPrescalar);
    _isTicking = true;
}

void Timer::stop()
{
    applyPrescalar(TimerPrescalar::STOPPED);
    _isTicking = false;
}

bool Timer::isRunning() const
{
    return _isTicking;
}

void Timer::setInterrupts(TimerInterrupt interrupts)
{
    switch (interrupts)
    {
    case TimerInterrupt::NONE:
        *_params.interruptMask &= ~(1 << OCIE0A | 1 << OCIE0B);
        break;

    case TimerInterrupt::COMPARE_A:
        *_params.interruptMask &= ~(1 << OCIE0B);
        *_params.interruptMask |= (1 << OCIE0A);
        break;

    case TimerInterrupt::COMPARE_B:
        *_params.interruptMask &= ~(1 << OCIE0A);
        *_params.interruptMask |= (1 << OCIE0B);
        break;

    case TimerInterrupt::BOTH:
        *_params.interruptMask |= (1 << OCIE0A | 1 << OCIE0B);
        break;
    }
}

void Timer::setPrescalar(TimerPrescalar prescalar)
{
    _runningPrescalar = prescalar;
    if (isRunning())
        applyPrescalar(prescalar);
}

void Timer::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        *_params.controlB &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        *_params.controlB &= ~(1 << CS01 | 1 << CS02);
        *_params.controlB |= (1 << CS00);
        break;

    case TimerPrescalar::EIGHT:
        *_params.controlB &= ~(1 << CS00 | 1 << CS02);
        *_params.controlB |= (1 << CS01);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        *_params.controlB &= ~(1 << CS02);
        *_params.controlB |= (1 << CS00 | 1 << CS01);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        *_params.controlB &= ~(1 << CS00 | 1 << CS01);
        *_params.controlB |= (1 << CS02);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        *_params.controlB &= ~(1 << CS01);
        *_params.controlB |= (1 << CS00 | 1 << CS02);
        break;
    }
}