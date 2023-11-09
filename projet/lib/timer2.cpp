#include "timer2.h"

Timer2::Timer2()
{
    _isTicking = false;
    _waveMode = TimerWaveMode::NORMAL;
    _interrupt = TimerInterrupt::NONE;
    _prescalar = TimerPrescalar::STOPPED;
}

void Timer2::start()
{
    cli();
    _isTicking = true;
    applyInterrupt(_interrupt);
    applyPrescalar(_prescalar);
    sei();
}

void Timer2::stop()
{
    cli();
    applyPrescalar(TimerPrescalar::STOPPED);
    applyInterrupt(TimerInterrupt::NONE);
    _isTicking = false;
    sei();
}

bool Timer2::isRunning() const
{
    return _isTicking;
}

void Timer2::setCounterValue(uint8_t value)
{
    cli();
    TCNT2 = value;
    sei();
}

void Timer2::setCompareValue(TimerCompare compare, uint8_t value)
{
    cli();
    switch (compare)
    {
        case TimerCompare::A:
            OCR2A = value;
            break;
        case TimerCompare::B:
            OCR2B = value;
            break;
    }
    sei();
}

void Timer2::setWaveMode(TimerWaveMode mode)
{
    cli();
    _waveMode = mode;

    switch (_waveMode)
    {
        case TimerWaveMode::NORMAL:
            TCCR2A &= ~(1 << WGM20 | 1 << WGM21);
            TCCR2B &= ~(1 << WGM22);
            break;

        case TimerWaveMode::CTC:
            TCCR2A &= ~(1 << WGM20);
            TCCR2A |= (1 << WGM21);
            TCCR2B &= ~(1 << WGM22);
            break;

        case TimerWaveMode::PWM_PHASE_CORRECT:
            TCCR2A &= ~(1 << WGM21);
            TCCR2A |= (1 << WGM20);
            TCCR2B &= ~(1 << WGM22);
            break;
    }
    sei();
}

void Timer2::setCompareMode(TimerCompare compare, TimerCompareMode mode)
{
    cli();
    Flag compareFlag0 = ((compare == TimerCompare::A) ? COM2A0 : COM2B0);
    Flag compareFlag1 = ((compare == TimerCompare::A) ? COM2A1 : COM2B1);

    switch (mode)
    {
        case TimerCompareMode::DISCONNECTED:
            TCCR2A &= ~(1 << compareFlag0 | 1 << compareFlag1);
            break;

        case TimerCompareMode::TOGGLE:
            if (_waveMode != TimerWaveMode::PWM_PHASE_CORRECT)
            {
                TCCR2A &= ~(1 << compareFlag1);
                TCCR2A |= (1 << compareFlag0);
            }
            break;

        case TimerCompareMode::CLEAR:
            TCCR2A &= ~(1 << compareFlag0);
            TCCR2A |= (1 << compareFlag1);
            break;
    }
    sei();
}

void Timer2::setInterrupt(TimerInterrupt interrupt)
{
    _interrupt = interrupt;
    if (isRunning())
    {
        cli();
        applyInterrupt(_interrupt);
        sei();
    }
}

void Timer2::setPrescalar(TimerPrescalar prescalar)
{
    _prescalar = prescalar;

    if (prescalar == TimerPrescalar::STOPPED)
        stop();

    if (isRunning())
    {
        cli();
        applyPrescalar(_prescalar);
        sei();
    }
}

void Timer2::applyInterrupt(TimerInterrupt interrupt)
{
    switch (interrupt)
    {
        case TimerInterrupt::NONE:
            TIMSK2 &= ~(1 << OCIE2A | 1 << OCIE2B);
            break;

        case TimerInterrupt::COMPARE_A:
            TIMSK2 &= ~(1 << OCIE2B);
            TIMSK2 |= (1 << OCIE2A);
            break;

        case TimerInterrupt::COMPARE_B:
            TIMSK2 &= ~(1 << OCIE2A);
            TIMSK2 |= (1 << OCIE2B);
            break;

        case TimerInterrupt::BOTH:
            TIMSK2 |= (1 << OCIE2A | 1 << OCIE2B);
            break;
    }
}

void Timer2::applyPrescalar(TimerPrescalar prescalar)
{
    switch (prescalar)
    {
        case TimerPrescalar::STOPPED:
            TCCR2B &= ~(1 << CS20 | 1 << CS21 | 1 << CS22);
            break;

        case TimerPrescalar::NO_PRESCALAR:
            TCCR2B &= ~(1 << CS21 | 1 << CS22);
            TCCR2B |= (1 << CS20);
            break;

        case TimerPrescalar::EIGHT:
            TCCR2B &= ~(1 << CS20 | 1 << CS22);
            TCCR2B |= (1 << CS21);
            break;

        case TimerPrescalar::THIRTY_TWO:
            TCCR2B &= ~(1 << CS22);
            TCCR2B |= (1 << CS20 | 1 << CS21);
            break;

        case TimerPrescalar::SIXTY_FOUR:
            TCCR2B &= ~(1 << CS20 | 1 << CS21);
            TCCR2B |= (1 << CS22);
            break;

        case TimerPrescalar::ONE_TWENTY_EIGHT:
            TCCR2B &= ~(1 << CS21);
            TCCR2B |= (1 << CS20 | 1 << CS22);
            break;

        case TimerPrescalar::TWO_FIFTY_SIX:
            TCCR2B &= ~(1 << CS20);
            TCCR2B |= (1 << CS21 | 1 << CS22);
            break;

        case TimerPrescalar::THOUSAND_TWENTY_FOUR:
            TCCR2B |= (1 << CS20 | 1 << CS21 | 1 << CS22);
            break;
    }
}
