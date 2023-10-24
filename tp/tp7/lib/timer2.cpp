#include "timer2.h"

Timer2::Timer2(TimerParameters parameters)
    : _params(parameters)
{
    _isTicking = false;
    _counter = &TCNT2;
    _compareA = &OCR2A;
    _compareB = &OCR2B;
    _waveMode = TimerWaveMode::NORMAL;
    _runningInterrupt = TimerInterrupt::NONE;
    _runningPrescalar = TimerPrescalar::STOPPED;
}

void Timer2::start()
{
    _isTicking = true;
    applyInterrupt(_runningInterrupt);
    applyPrescalar(_runningPrescalar);
}

void Timer2::stop()
{
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
}

bool Timer2::isRunning() const
{
    return _isTicking;
}

void Timer2::setCounterValue(uint8_t value)
{
    *_counter = value;
}

void Timer2::setCompareValue(TimerCompare compare, uint8_t value)
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

void Timer2::setWaveMode(TimerWaveMode mode)
{
    _waveMode = mode;

    switch (_waveMode)
    {
    case TimerWaveMode::NORMAL:
        *_params._controlA &= ~(1 << WGM20 | 1 << WGM21);
        *_params._controlB &= ~(1 << WGM22);
        break;

    case TimerWaveMode::CTC:
        *_params._controlA &= ~(1 << WGM20 | 1 << WGM21);
        *_params._controlB |= (1 << WGM22);
        break;

    case TimerWaveMode::PWM_PHASE_CORRECT:
        *_params._controlA &= ~(1 << WGM21);
        *_params._controlA |= (1 << WGM20);
        *_params._controlB &= ~(1 << WGM22);
        break;
    }
}

void Timer2::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    uint8_t compareFlag0 = ((compare == TimerCompare::A) ? COM2A0 : COM2B0);
    uint8_t compareFlag1 = ((compare == TimerCompare::A) ? COM2A1 : COM2B1);

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

void Timer2::setInterrupt(TimerInterrupt interrupt)
{
    _runningInterrupt = interrupt;
    if (isRunning())
        applyInterrupt(_runningInterrupt);
}

void Timer2::setPrescalar(TimerPrescalar prescalar)
{
    if (prescalar == TimerPrescalar::STOPPED) 
        stop();

    _runningPrescalar = prescalar;
    if (isRunning())
        applyPrescalar(_runningPrescalar);
}

void Timer2::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
    case TimerInterrupt::NONE:
        *_params._interruptMask &= ~(1 << OCIE2A | 1 << OCIE2B);
        break;

    case TimerInterrupt::COMPARE_A:
        *_params._interruptMask &= ~(1 << OCIE2B);
        *_params._interruptMask |= (1 << OCIE2A);
        break;

    case TimerInterrupt::COMPARE_B:
        *_params._interruptMask &= ~(1 << OCIE2A);
        *_params._interruptMask |= (1 << OCIE2B);
        break;

    case TimerInterrupt::BOTH:
        *_params._interruptMask |= (1 << OCIE2A | 1 << OCIE2B);
        break;
    }
}

void Timer2::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        *_params._controlB &= ~(1 << CS20 | 1 << CS21 | 1 << CS22);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        *_params._controlB &= ~(1 << CS21 | 1 << CS22);
        *_params._controlB |= (1 << CS20);
        break;

    case TimerPrescalar::EIGHT:
        *_params._controlB &= ~(1 << CS20 | 1 << CS22);
        *_params._controlB |= (1 << CS21);
        break;

        case TimerPrescalar::THIRTY_TWO:
        *_params._controlB &= ~(1 << CS22);
        *_params._controlB |= (1 << CS20 | 1 << CS21);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        *_params._controlB &= ~(1 << CS20 | 1 << CS21);
        *_params._controlB |= (1 << CS22);
        break;

    case TimerPrescalar::ONE_TWENTY_EIGHT:
        *_params._controlB &= ~(1 << CS21);
        *_params._controlB |= (1 << CS20 | 1 << CS22);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        *_params._controlB &= ~(1 << CS20);
        *_params._controlB |= (1 << CS21 | 1 << CS22);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        *_params._controlB |= (1 << CS20 | 1 << CS21 | 1 << CS22);
        break;
    }
}