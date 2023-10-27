#include "timer0.h"

Timer0::Timer0(TimerControls parameters)
    : _controls(parameters)
{
    _isTicking = false;
    _counter = &TCNT0;
    _compareA = &OCR0A;
    _compareB = &OCR0B;
    _settings._waveMode = TimerWaveMode::NORMAL;
    _settings._interrupt = TimerInterrupt::NONE;
    _settings._prescalar = TimerPrescalar::STOPPED;
}

void Timer0::start()
{
    _isTicking = true;
    applyInterrupt(_settings._interrupt);
    applyPrescalar(_settings._prescalar);
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
    _settings._waveMode = mode;

    switch (_settings._waveMode)
    {
    case TimerWaveMode::NORMAL:
        *_controls._controlA &= ~(1 << WGM00 | 1 << WGM01);
        *_controls._controlB &= ~(1 << WGM02);
        break;

    case TimerWaveMode::CTC:
        *_controls._controlA &= ~(1 << WGM00);
        *_controls._controlA |= (1 << WGM01);
        *_controls._controlB &= ~(1 << WGM02);
        break;

    case TimerWaveMode::PWM_PHASE_CORRECT:
        *_controls._controlA &= ~(1 << WGM01);
        *_controls._controlA |= (1 << WGM00);
        *_controls._controlB &= ~(1 << WGM02);
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
        *_controls._controlA &= ~(1 << compareFlag0 | 1 << compareFlag1);
        break;

    case TimerCompareMode::TOGGLE:
        if (_settings._waveMode != TimerWaveMode::PWM_PHASE_CORRECT)
        {
        *_controls._controlA &= ~(1 << compareFlag1);
        *_controls._controlA |= (1 << compareFlag0);
        }
        break;

    case TimerCompareMode::CLEAR:
            *_controls._controlA &= ~(1 << compareFlag0);
            *_controls._controlA |= (1 << compareFlag1);
        break;
    }
}

void Timer0::setInterrupt(TimerInterrupt interrupt)
{
    _settings._interrupt = interrupt;
    if (isRunning())
        applyInterrupt(_settings._interrupt);
}

void Timer0::setPrescalar(TimerPrescalar prescalar)
{
    if (prescalar == TimerPrescalar::STOPPED) 
        stop();

    _settings._prescalar = prescalar;
    if (isRunning())
        applyPrescalar(_settings._prescalar);
}

void Timer0::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
    case TimerInterrupt::NONE:
        *_controls._interruptMask &= ~(1 << OCIE1A | 1 << OCIE1B);
        break;

    case TimerInterrupt::COMPARE_A:
        *_controls._interruptMask &= ~(1 << OCIE1B);
        *_controls._interruptMask |= (1 << OCIE1A);
        break;

    case TimerInterrupt::COMPARE_B:
        *_controls._interruptMask &= ~(1 << OCIE1A);
        *_controls._interruptMask |= (1 << OCIE1B);
        break;

    case TimerInterrupt::BOTH:
        *_controls._interruptMask |= (1 << OCIE1A | 1 << OCIE1B);
        break;
    }
}

void Timer0::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        *_controls._controlB &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        *_controls._controlB &= ~(1 << CS01 | 1 << CS02);
        *_controls._controlB |= (1 << CS00);
        break;

    case TimerPrescalar::EIGHT:
        *_controls._controlB &= ~(1 << CS00 | 1 << CS02);
        *_controls._controlB |= (1 << CS01);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        *_controls._controlB &= ~(1 << CS02);
        *_controls._controlB |= (1 << CS00 | 1 << CS01);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        *_controls._controlB &= ~(1 << CS00 | 1 << CS01);
        *_controls._controlB |= (1 << CS02);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        *_controls._controlB &= ~(1 << CS01);
        *_controls._controlB |= (1 << CS00 | 1 << CS02);
        break;
    default: break;
    }
}