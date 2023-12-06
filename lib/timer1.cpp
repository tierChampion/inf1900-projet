#include "timer1.h"

Timer1::Timer1()
{
    _isTicking = false;
    _waveMode = TimerWaveMode::NORMAL;
    _interrupt = TimerInterrupt::NONE;
    _prescalar = TimerPrescalar::STOPPED;
}

void Timer1::start()
{
    cli();
    _isTicking = true;
    applyInterrupt(_interrupt);
    applyPrescalar(_prescalar);
    sei();
}

void Timer1::stop()
{
    cli();
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
    sei();
}

bool Timer1::isRunning() const
{
    return _isTicking;
}

void Timer1::setCounterValue(uint16_t value)
{
    cli();
    TCNT1 = value;
    sei();
}

void Timer1::setCompareValue(TimerCompare compare, uint16_t value)
{
    cli();
    switch (compare)
    {
        case TimerCompare::A:
            OCR1A = value;
            break;
        case TimerCompare::B:
            OCR1B = value;
            break;
    }
    sei();
}

void Timer1::setWaveMode(TimerWaveMode mode)
{
    cli();
    _waveMode = mode;

    switch (_waveMode)
    {
        case TimerWaveMode::NORMAL:
            TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
            TCCR1B &= ~(1 << WGM12 | 1 << WGM13);
            break;

        case TimerWaveMode::CTC:
            TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
            TCCR1B &= ~(1 << WGM13);
            TCCR1B |= (1 << WGM12);
            break;

        case TimerWaveMode::PWM_PHASE_CORRECT:
            TCCR1A &= ~(1 << WGM11);
            TCCR1A |= (1 << WGM10);
            TCCR1B &= ~(1 << WGM12 | 1 << WGM13);
            break;
    }
    sei();
}

void Timer1::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    cli();
    Flag compareFlag0 = ((compare == TimerCompare::A) ? COM1A0 : COM1B0);
    Flag compareFlag1 = ((compare == TimerCompare::A) ? COM1A1 : COM1B1);

    switch (mode)
    {
        case TimerCompareMode::DISCONNECTED:
            TCCR1A &= ~(1 << compareFlag0 | 1 << compareFlag1);
            break;

        case TimerCompareMode::TOGGLE:
            if (_waveMode != TimerWaveMode::PWM_PHASE_CORRECT)
            {
                TCCR1A |= (1 << compareFlag0);
                TCCR1A &= ~(1 << compareFlag1);
            }
            break;

        case TimerCompareMode::CLEAR:
            TCCR1A &= ~(1 << compareFlag0);
            TCCR1A |= (1 << compareFlag1);
            break;
    }
    sei();
}

void Timer1::setInterrupt(TimerInterrupt interrupt)
{
    _interrupt = interrupt;
    if (isRunning())
    {
        cli();
        applyInterrupt(_interrupt);
        sei();
    }
}

void Timer1::setPrescalar(TimerPrescalar prescalar)
{
    _prescalar = prescalar;

    if (prescalar == TimerPrescalar::STOPPED)
        stop();
    else if (isRunning())
    {
        cli();
        applyPrescalar(_prescalar);
        sei();
    }
}

void Timer1::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
        case TimerInterrupt::NONE:
            TIMSK1 &= ~(1 << OCIE1A | 1 << OCIE1B);
            break;

        case TimerInterrupt::COMPARE_A:
            TIMSK1 &= ~(1 << OCIE1B);
            TIMSK1 |= (1 << OCIE1A);
            break;

        case TimerInterrupt::COMPARE_B:
            TIMSK1 &= ~(1 << OCIE1A);
            TIMSK1 |= (1 << OCIE1B);
            break;

        case TimerInterrupt::BOTH:
            TIMSK1 |= (1 << OCIE1A | 1 << OCIE1B);
            break;
    }
}

void Timer1::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
        case TimerPrescalar::STOPPED:
            TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
            break;

        case TimerPrescalar::NO_PRESCALAR:
            TCCR1B &= ~(1 << CS11 | 1 << CS12);
            TCCR1B |= (1 << CS10);
            break;

        case TimerPrescalar::EIGHT:
            TCCR1B &= ~(1 << CS10 | 1 << CS12);
            TCCR1B |= (1 << CS11);
            break;

        case TimerPrescalar::SIXTY_FOUR:
            TCCR1B &= ~(1 << CS12);
            TCCR1B |= (1 << CS10 | 1 << CS11);
            break;

        case TimerPrescalar::TWO_FIFTY_SIX:
            TCCR1B &= ~(1 << CS10 | 1 << CS11);
            TCCR1B |= (1 << CS12);
            break;

        case TimerPrescalar::THOUSAND_TWENTY_FOUR:
            TCCR1B &= ~(1 << CS11);
            TCCR1B |= (1 << CS10 | 1 << CS12);
            break;
        default:
            break;
    }
}
