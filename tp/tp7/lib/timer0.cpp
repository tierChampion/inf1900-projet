#include "timer0.h"

Timer0::Timer0()
{
    _isTicking = false;
    _waveMode = TimerWaveMode::NORMAL;
    _interrupt = TimerInterrupt::NONE;
    _prescalar = TimerPrescalar::STOPPED;
}

void Timer0::start()
{
    cli();
    _isTicking = true;
    applyInterrupt(_interrupt);
    applyPrescalar(_prescalar);
    sei();
}

void Timer0::stop()
{
    cli();
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
    sei();
}

bool Timer0::isRunning() const
{
    return _isTicking;
}

void Timer0::setCounterValue(uint8_t value)
{
    cli();
    TCNT0 = value;
    sei();
}

void Timer0::setCompareValue(TimerCompare compare, uint8_t value)
{
    cli();
    switch (compare)
    {
    case TimerCompare::A:
        OCR0A = value;
        break;
    case TimerCompare::B:
        OCR0B = value;
        break;
    }
    sei();
}

void Timer0::setWaveMode(TimerWaveMode mode)
{
    cli();
    _waveMode = mode;

    switch (_waveMode)
    {
    case TimerWaveMode::NORMAL:
        TCCR0A &= ~(1 << WGM00 | 1 << WGM01);
        TCCR0B &= ~(1 << WGM02);
        break;

    case TimerWaveMode::CTC:
        TCCR0A &= ~(1 << WGM00);
        TCCR0A |= (1 << WGM01);
        TCCR0B &= ~(1 << WGM02);
        break;

    case TimerWaveMode::PWM_PHASE_CORRECT:
        TCCR0A &= ~(1 << WGM01);
        TCCR0A |= (1 << WGM00);
        TCCR0B &= ~(1 << WGM02);
        break;
    }
    sei();
}

void Timer0::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    cli();
    Flag compareFlag0 = ((compare == TimerCompare::A) ? COM0A0 : COM0B0);
    Flag compareFlag1 = ((compare == TimerCompare::A) ? COM0A1 : COM0B1);

    switch (mode)
    {
    case TimerCompareMode::DISCONNECTED:
        TCCR0A &= ~(1 << compareFlag0 | 1 << compareFlag1);
        break;

    case TimerCompareMode::TOGGLE:
        if (_waveMode != TimerWaveMode::PWM_PHASE_CORRECT)
        {
            TCCR0A &= ~(1 << compareFlag1);
            TCCR0A |= (1 << compareFlag0);
        }
        break;

    case TimerCompareMode::CLEAR:
        TCCR0A &= ~(1 << compareFlag0);
        TCCR0A |= (1 << compareFlag1);
        break;
    }
    sei();
}

void Timer0::setInterrupt(TimerInterrupt interrupt)
{
    _interrupt = interrupt;
    if (isRunning()) {
        cli();
        applyInterrupt(_interrupt);
        sei();
    }
}

void Timer0::setPrescalar(TimerPrescalar prescalar)
{
    _prescalar = prescalar;

    if (prescalar == TimerPrescalar::STOPPED)
        stop();

    else if (isRunning()) {
        cli();
        applyPrescalar(_prescalar);
        sei();
    }
}

void Timer0::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
    case TimerInterrupt::NONE:
        TIMSK0 &= ~(1 << OCIE1A | 1 << OCIE1B);
        break;

    case TimerInterrupt::COMPARE_A:
        TIMSK0 &= ~(1 << OCIE1B);
        TIMSK0 |= (1 << OCIE1A);
        break;

    case TimerInterrupt::COMPARE_B:
        TIMSK0 &= ~(1 << OCIE1A);
        TIMSK0 |= (1 << OCIE1B);
        break;

    case TimerInterrupt::BOTH:
        TIMSK0 |= (1 << OCIE1A | 1 << OCIE1B);
        break;
    }
}

void Timer0::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
    case TimerPrescalar::STOPPED:
        TCCR0B &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
        break;

    case TimerPrescalar::NO_PRESCALAR:
        TCCR0B &= ~(1 << CS01 | 1 << CS02);
        TCCR0B |= (1 << CS00);
        break;

    case TimerPrescalar::EIGHT:
        TCCR0B &= ~(1 << CS00 | 1 << CS02);
        TCCR0B |= (1 << CS01);
        break;

    case TimerPrescalar::SIXTY_FOUR:
        TCCR0B &= ~(1 << CS02);
        TCCR0B |= (1 << CS00 | 1 << CS01);
        break;

    case TimerPrescalar::TWO_FIFTY_SIX:
        TCCR0B &= ~(1 << CS00 | 1 << CS01);
        TCCR0B |= (1 << CS02);
        break;

    case TimerPrescalar::THOUSAND_TWENTY_FOUR:
        TCCR0B &= ~(1 << CS01);
        TCCR0B |= (1 << CS00 | 1 << CS02);
        break;
    default:
        break;
    }
}
