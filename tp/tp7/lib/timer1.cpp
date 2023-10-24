#include "timer1.h"

Timer1::Timer1(TimerParameters parameters)
    : _params(parameters)
{
    _isTicking = false;
    _waveMode = TimerWaveMode::NORMAL;
    _runningInterrupt = TimerInterrupt::NONE;
    _runningPrescalar = TimerPrescalar::STOPPED;
}

void Timer1::start()
{
    _isTicking = true;
    applyInterrupt(_runningInterrupt);
    applyPrescalar(_runningPrescalar);
}

void Timer1::stop()
{
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
}

bool Timer1::isRunning() const
{
    return _isTicking;
}

void Timer1::setCounterValue(uint16_t value)
{
    *_counter = value;
}

void Timer1::setCompareValue(TimerCompare compare, uint16_t value)
{
    switch (compare)
    {
    case TimerCompare::A:
        *_compareA = value;
        break;
    case TimerCompare::B:
        *_compareB = value; 
        break;
    }
}

void Timer1::setWaveMode(TimerWaveMode mode)
{
    _waveMode = mode;

    switch (_waveMode)
    {
    case TimerWaveMode::NORMAL:
        *_params._controlA &= ~(1 << WGM10 | 1 << WGM11);
        *_params._controlB &= ~(1 << WGM12 | 1 << WGM13);
        break;

    case TimerWaveMode::CTC:
        *_params._controlA &= ~(1 << WGM10 | 1 << WGM11);
        *_params._controlB &= ~(1 << WGM13);
        *_params._controlB |= (1 << WGM12);
        break;

    case TimerWaveMode::PWM_PHASE_CORRECT:
        *_params._controlA &= ~(1 << WGM11);
        *_params._controlA |= (1 << WGM10);
        *_params._controlB &= ~(1 << WGM12 | 1 << WGM13);
        break;
    }
}

void Timer1::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    uint8_t compareFlag0 = ((compare == TimerCompare::A) ? COM1A0 : COM1B0);
    uint8_t compareFlag1 = ((compare == TimerCompare::A) ? COM1A1 : COM1B1);

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

void Timer1::setInterrupt(TimerInterrupt interrupt)
{
    _runningInterrupt = interrupt;
    if (isRunning())
        applyInterrupt(_runningInterrupt);
}

void Timer1::setPrescalar(TimerPrescalar prescalar)
{
    if (prescalar == TimerPrescalar::STOPPED) 
        stop();

    _runningPrescalar = prescalar;
    if (isRunning())
        applyPrescalar(_runningPrescalar);
}

void Timer1::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
    case TimerInterrupt::NONE:
        *_params._interruptMask &= ~(1 << OCIE1A | 1 << OCIE1B);
        break;

    case TimerInterrupt::COMPARE_A:
        *_params._interruptMask &= ~(1 << OCIE1B);
        *_params._interruptMask |= (1 << OCIE1A);
        break;

    case TimerInterrupt::COMPARE_B:
        *_params._interruptMask &= ~(1 << OCIE1A);
        *_params._interruptMask |= (1 << OCIE1B);
        break;

    case TimerInterrupt::BOTH:
        *_params._interruptMask |= (1 << OCIE1A | 1 << OCIE1B);
        break;
    }
}

void Timer1::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        *_params._controlB &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        *_params._controlB &= ~(1 << CS11 | 1 << CS12);
        *_params._controlB |= (1 << CS10);
        break;

    case TimerPrescalar::EIGHT:
        *_params._controlB &= ~(1 << CS10 | 1 << CS12);
        *_params._controlB |= (1 << CS11);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        *_params._controlB &= ~(1 << CS12);
        *_params._controlB |= (1 << CS10 | 1 << CS11);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        *_params._controlB &= ~(1 << CS10 | 1 << CS11);
        *_params._controlB |= (1 << CS12);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        *_params._controlB &= ~(1 << CS11);
        *_params._controlB |= (1 << CS10 | 1 << CS12);
        break;
    default: break;
    }
}