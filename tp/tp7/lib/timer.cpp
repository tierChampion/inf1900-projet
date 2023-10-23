#include "timer.h"

Timer::Timer(TimerParameters parameters)
    : _params(parameters)
{
    _isTicking = false;
    setWaveMode(TimerWaveMode::NORMAL);
    setInterrupt(TimerInterrupt::NONE);
    setPrescalar(TimerPrescalar::STOPPED);
}

void Timer::start()
{
    _isTicking = true;
    applyInterrupt(_runningInterrupt);
    applyPrescalar(_runningPrescalar);
}

void Timer::stop()
{
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
}

bool Timer::isRunning() const
{
    return _isTicking;
}

void Timer::setWaveMode(TimerWaveMode mode)
{
    _waveMode = mode;

    switch (_waveMode)
    {
        case TimerWaveMode::NORMAL:
        *_params.controlA &= ~(1 << WGM00 | 1 << WGM01);
        *_params.controlB &= ~(1 << WGM02);
        break;

        case TimerWaveMode::CTC:
        *_params.controlA &= ~(1 << WGM00);
        *_params.controlA |= (1 << WGM01);
        *_params.controlB &= ~(1 << WGM02);
        break;

        case TimerWaveMode::PWM_PHASE_CORRECT:
        *_params.controlA &= ~(1 << WGM01);
        *_params.controlA |= (1 << WGM00);
        *_params.controlB &= ~(1 << WGM02);
        break;
    }
}

void Timer::setInterrupt(TimerInterrupt interrupt)
{
    _runningInterrupt = interrupt;
    if (isRunning())
        applyInterrupt(_runningInterrupt);
}

void Timer::setPrescalar(TimerPrescalar prescalar)
{
    _runningPrescalar = prescalar;
    if (isRunning())
        applyPrescalar(_runningPrescalar);
}

void Timer::applyInterrupt(TimerInterrupt interrupt) {
switch (interrupt)
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