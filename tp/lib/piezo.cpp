#include "piezo.h"

const uint8_t STARTING_NOTE = 45;
const uint8_t NOTES_PER_OCTAVE = 12;
const uint8_t OCTAVE_COUNT = 4;

Piezo::Piezo()
    : _timer(Timer2()), _timerPin(WritePin(Port::D, PD7)), _groundPin(WritePin(Port::D, PD6))
{
    _timerPin.set();
    _groundPin.clear();
    _timer.setWaveMode(TimerWaveMode::CTC);
    _timer.setCompareMode(TimerCompare::A, TimerCompareMode::TOGGLE);
}

void Piezo::play(uint8_t val)
{
    uint8_t baseVal = val - STARTING_NOTE;
    uint8_t noteVal = baseVal % NOTES_PER_OCTAVE;
    uint8_t octaveVal = (baseVal / NOTES_PER_OCTAVE) % OCTAVE_COUNT;

    Note note = static_cast<Note>(noteVal);
    Octave octave = static_cast<Octave>(octaveVal);

    play(note, octave);

    _timer.start();
}

void Piezo::play(Note note, Octave octave)
{
    switch (note)
    {
    case Note::A:
        _timer.setCompareValue(TimerCompare::A, 142);
        break;
    case Note::ASHARP_BFLAT:
        _timer.setCompareValue(TimerCompare::A, 134);
        break;
    case Note::B:
        _timer.setCompareValue(TimerCompare::A, 127);
        break;
    case Note::C:
        _timer.setCompareValue(TimerCompare::A, 120);
        break;
    case Note::CSHARP_DFLAT:
        _timer.setCompareValue(TimerCompare::A, 113);
        break;
    case Note::D:
        _timer.setCompareValue(TimerCompare::A, 107);
        break;
    case Note::DSHARP_EFLAT:
        _timer.setCompareValue(TimerCompare::A, 100);
        break;
    case Note::E:
        _timer.setCompareValue(TimerCompare::A, 95);
        break;
    case Note::F:
        _timer.setCompareValue(TimerCompare::A, 89);
        break;
    case Note::FSHARP_GFLAT:
        _timer.setCompareValue(TimerCompare::A, 84);
        break;
    case Note::G:
        _timer.setCompareValue(TimerCompare::A, 80);
        break;
    case Note::GSHARP_AFLAT:
        _timer.setCompareValue(TimerCompare::A, 75);
        break;
    }

    switch (octave)
    {
    case Octave::LOW:
    {
        _timer.setPrescalar(TimerPrescalar::TWO_FIFTY_SIX);
        break;
    }
    case Octave::MEDIUM:
    {
        _timer.setPrescalar(TimerPrescalar::ONE_TWENTY_EIGHT);
        break;
    }
    case Octave::HIGH:
    {
        _timer.setPrescalar(TimerPrescalar::SIXTY_FOUR);
        break;
    }
    case Octave::TOP:
    {
        _timer.setPrescalar(TimerPrescalar::THIRTY_TWO);
        break;
    }
    }
    _timer.start();
}

void Piezo::stop()
{
    _timer.setCompareValue(TimerCompare::A, 0);
    _timer.stop();
}