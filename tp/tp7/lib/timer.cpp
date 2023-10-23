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