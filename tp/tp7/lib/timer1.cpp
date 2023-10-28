#include "timer1.h"

Timer1::Timer1(TimerControls parameters)
    : _controls(parameters)
{
    _isTicking = false;
    _counter = &TCNT1;
    _compareA = &OCR1A;
    _compareB = &OCR1A;
    _settings._waveMode = TimerWaveMode::NORMAL;
    _settings._interrupt = TimerInterrupt::NONE;
    _settings._prescalar = TimerPrescalar::STOPPED;
}

void Timer1::start()
{
    _isTicking = true;
    applyInterrupt(_settings._interrupt);
    applyPrescalar(_settings._prescalar);
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
    _settings._waveMode = mode;

    switch (_settings._waveMode)
    {
    case TimerWaveMode::NORMAL:
        *_controls._controlA &= ~(1 << WGM10 | 1 << WGM11);
        *_controls._controlB &= ~(1 << WGM12 | 1 << WGM13);
        break;

    case TimerWaveMode::CTC:
        *_controls._controlA &= ~(1 << WGM10 | 1 << WGM11);
        *_controls._controlB &= ~(1 << WGM13);
        *_controls._controlB |= (1 << WGM12);
        break;

    case TimerWaveMode::PWM_PHASE_CORRECT:
        *_controls._controlA &= ~(1 << WGM11);
        *_controls._controlA |= (1 << WGM10);
        *_controls._controlB &= ~(1 << WGM12 | 1 << WGM13);
        break;
    }
}

void Timer1::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    Flag compareFlag0 = ((compare == TimerCompare::A) ? COM1A0 : COM1B0);
    Flag compareFlag1 = ((compare == TimerCompare::A) ? COM1A1 : COM1B1);

    switch (mode)
    {
    case TimerCompareMode::DISCONNECTED:
        *_controls._controlA &= ~(1 << compareFlag0 | 1 << compareFlag1);
        break;

    case TimerCompareMode::TOGGLE:
        if (_settings._waveMode != TimerWaveMode::PWM_PHASE_CORRECT)
        {
            *_controls._controlA |= (1 << compareFlag0);
            *_controls._controlA &= ~(1 << compareFlag1);
        }
        break;

    case TimerCompareMode::CLEAR:
        *_controls._controlA &= ~(1 << compareFlag0);
        *_controls._controlA |= (1 << compareFlag1);
        break;
    }
}

void Timer1::setInterrupt(TimerInterrupt interrupt)
{
    _settings._interrupt = interrupt;
    if (isRunning())
        applyInterrupt(_settings._interrupt);
}

void Timer1::setPrescalar(TimerPrescalar prescalar)
{
    if (prescalar == TimerPrescalar::STOPPED) 
        stop();

    _settings._prescalar = prescalar;
    if (isRunning())
        applyPrescalar(_settings._prescalar);
}

void Timer1::applyInterrupt(TimerInterrupt interrupt)
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

void Timer1::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        *_controls._controlB &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        *_controls._controlB &= ~(1 << CS11 | 1 << CS12);
        *_controls._controlB |= (1 << CS10);
        break;

    case TimerPrescalar::EIGHT:
        *_controls._controlB &= ~(1 << CS10 | 1 << CS12);
        *_controls._controlB |= (1 << CS11);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        *_controls._controlB &= ~(1 << CS12);
        *_controls._controlB |= (1 << CS10 | 1 << CS11);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        *_controls._controlB &= ~(1 << CS10 | 1 << CS11);
        *_controls._controlB |= (1 << CS12);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        *_controls._controlB &= ~(1 << CS11);
        *_controls._controlB |= (1 << CS10 | 1 << CS12);
        break;
    default: break;
    }
}