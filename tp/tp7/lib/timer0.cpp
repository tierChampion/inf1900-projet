#include "timer0.h"

Timer0::Timer0(TimerParameters parameters)
    : _params(parameters)
{
    _isTicking = false;
    _counter = &TCNT0;
    _compareA = &OCR0A;
    _compareB = &OCR0B;
    _waveMode = TimerWaveMode::NORMAL;
    _runningInterrupt = TimerInterrupt::NONE;
    _runningPrescalar = TimerPrescalar::STOPPED;
}

void Timer0::start()
{
    _isTicking = true;
    applyInterrupt(_runningInterrupt);
    applyPrescalar(_runningPrescalar);
}

void Timer0::stop()
{
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
}

bool Timer0::isRunning() const
{
    return _isTicking;
}

void Timer0::setCounterValue(uint8_t value)
{
    *_counter = value;
}

void Timer0::setCompareValue(TimerCompare compare, uint8_t value)
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

void Timer0::setWaveMode(TimerWaveMode mode)
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

void Timer0::setCompareMode(TimerCompare compare, TimerCompareMode mode)
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
        *_params._controlA &= ~(1 << compareFlag1);
        *_params._controlA |= (1 << compareFlag0);
        }
        break;

    case TimerCompareMode::CLEAR:
            *_params._controlA &= ~(1 << compareFlag0);
            *_params._controlA |= (1 << compareFlag1);
        break;
    }
}

void Timer0::setInterrupt(TimerInterrupt interrupt)
{
    _runningInterrupt = interrupt;
    if (isRunning())
        applyInterrupt(_runningInterrupt);
}

void Timer0::setPrescalar(TimerPrescalar prescalar)
{
    if (prescalar == TimerPrescalar::STOPPED) 
        stop();

    _runningPrescalar = prescalar;
    if (isRunning())
        applyPrescalar(_runningPrescalar);
}

void Timer0::applyInterrupt(TimerInterrupt interrupt)
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

void Timer0::applyPrescalar(TimerPrescalar prescalar)
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
    default: break;
    }
}