#include "piezo.h"

Piezo::Piezo()
    : _timer(Timer2()), _timerPin(WritePin(Port::D, PD7)), _groundPin(WritePin(Port::D, PD6))
{
    _timerPin.set();
    _groundPin.clear();
    _timer.setWaveMode(TimerWaveMode::CTC);
    _timer.setCompareMode(TimerCompare::A, TimerCompareMode::TOGGLE);
}

void Piezo::play(Note note, Octave octave)
{
    switch(note)
    {
        case Note::A:
            _timer.setCompareValue(TimerCompare::A, 142); // 149
        break;
        case Note::ASHARP:
        case Note::BFLAT:
            _timer.setCompareValue(TimerCompare::A, 134);
        break;
        case Note::B:
            _timer.setCompareValue(TimerCompare::A, 127);
        break;
        case Note::C:
            _timer.setCompareValue(TimerCompare::A, 120);
        break;
        case Note::CSHARP:
        case Note::DFLAT: 
            _timer.setCompareValue(TimerCompare::A, 113);
        break;
        case Note::D: 
            _timer.setCompareValue(TimerCompare::A, 107);
        break;
        case Note::DSHARP: 
        case Note::EFLAT: 
            _timer.setCompareValue(TimerCompare::A, 100);
        break;
        case Note::E: 
            _timer.setCompareValue(TimerCompare::A, 95);
        break;
        case Note::F: 
            _timer.setCompareValue(TimerCompare::A, 89);
        break;
        case Note::FSHARP: 
        case Note::GFLAT: 
            _timer.setCompareValue(TimerCompare::A, 84);
        break;
        case Note::G: 
            _timer.setCompareValue(TimerCompare::A, 80);
        break;
        case Note::GSHARP: 
        case Note::AFLAT: 
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
    }
    _timer.start();
}

void Piezo::stop() {
    _timer.setCompareValue(TimerCompare::A, 0);
    _timer.stop();
}