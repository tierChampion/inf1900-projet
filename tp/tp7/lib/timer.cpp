#include "timer.h"

Timer::Timer(TimerParameters parameters)
    : _params(parameters)
{
    _isTicking = false;
    _waveMode = TimerWaveMode::NORMAL;
    _runningInterrupt = TimerInterrupt::NONE;
    _runningPrescalar = TimerPrescalar::STOPPED;
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

void Timer::setCounterValue(uint16_t value)
{
    // TODO: check how to work with the 16 bits of the TIM1.
    //  does the _counter need to be a different type ?
    *_params._counter = (value & 0xFF);
}

void Timer::setCompareValue(TimerCompare compare, uint16_t value)
{
    // TODO: check how to work with the 16 bits of the TIM1.
    //  does the _counter need to be a different type ?
    switch (compare)
    {
    case TimerCompare::A:
        *_params._compareA = (value & 0xFF);
        break;
    case TimerCompare::B:
        *_params._compareB = (value & 0xFF);
        break;
    }
}

void Timer::setWaveMode(TimerWaveMode mode)
{
    _waveMode = mode;

    switch (_waveMode)
    {
    case TimerWaveMode::NORMAL:
        *_params._controlA &= ~(1 << WGM00 | 1 << WGM01);
        *_params._controlB &= ~(1 << WGM02);
        break;

    case TimerWaveMode::CTC:
        *_params._controlA &= ~(1 << WGM00);
        *_params._controlA |= (1 << WGM01);
        *_params._controlB &= ~(1 << WGM02);
        break;

    case TimerWaveMode::PWM_PHASE_CORRECT:
        *_params._controlA &= ~(1 << WGM01);
        *_params._controlA |= (1 << WGM00);
        *_params._controlB &= ~(1 << WGM02);
        break;
    }
}

void Timer::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    uint8_t compareFlag0 = ((compare == TimerCompare::A) ? COM0A0 : COM0B0);
    uint8_t compareFlag1 = ((compare == TimerCompare::A) ? COM0A1 : COM0B1);

    switch (mode)
    {
    case TimerCompareMode::DISCONNECTED:
        *_params._controlA &= ~(1 << compareFlag0 | 1 << compareFlag1);
        break;

    case TimerCompareMode::TOGGLE:
        if (_waveMode != TimerWaveMode::PWM_PHASE_CORRECT)
        {
            *_params._controlA &= ~(1 << compareFlag0);
            *_params._controlA |= (1 << compareFlag1);
        }
        break;

    case TimerCompareMode::CLEAR:
        *_params._controlA &= ~(1 << compareFlag1);
        *_params._controlA |= (1 << compareFlag0);
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
    // TODO: do we allow the setting of prescalar 0 ?
    //  would need to handle the stopping of the timer
    _runningPrescalar = prescalar;
    if (isRunning())
        applyPrescalar(_runningPrescalar);
}

void Timer::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
    case TimerInterrupt::NONE:
        *_params._interruptMask &= ~(1 << OCIE0A | 1 << OCIE0B);
        break;

    case TimerInterrupt::COMPARE_A:
        *_params._interruptMask &= ~(1 << OCIE0B);
        *_params._interruptMask |= (1 << OCIE0A);
        break;

    case TimerInterrupt::COMPARE_B:
        *_params._interruptMask &= ~(1 << OCIE0A);
        *_params._interruptMask |= (1 << OCIE0B);
        break;

    case TimerInterrupt::BOTH:
        *_params._interruptMask |= (1 << OCIE0A | 1 << OCIE0B);
        break;
    }
}

void Timer::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        *_params._controlB &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        *_params._controlB &= ~(1 << CS01 | 1 << CS02);
        *_params._controlB |= (1 << CS00);
        break;

    case TimerPrescalar::EIGHT:
        *_params._controlB &= ~(1 << CS00 | 1 << CS02);
        *_params._controlB |= (1 << CS01);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        *_params._controlB &= ~(1 << CS02);
        *_params._controlB |= (1 << CS00 | 1 << CS01);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        *_params._controlB &= ~(1 << CS00 | 1 << CS01);
        *_params._controlB |= (1 << CS02);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        *_params._controlB &= ~(1 << CS01);
        *_params._controlB |= (1 << CS00 | 1 << CS02);
        break;
    }
}