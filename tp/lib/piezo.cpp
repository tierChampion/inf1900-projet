#include "piezo.h"

Piezo::Piezo() 
    : _timer(Timer2()),_timerPin(WritePin(Port::D,7)), _groundPin(WritePin(Port::D, 6))
{
    _timerPin.set();
    _groundPin.clear();
    _timer.setWaveMode(TimerWaveMode::CTC);
    _timer.setCompareMode(TimerCompare::A, TimerCompareMode::TOGGLE);
}

void Piezo::play(Note note, Octave octave) {
    switch(note) 
    {
        case Note::A: {
            _timer.setCompareValue(TimerCompare::A, 142);
        }
        break;

        case Note::D: {
            _timer.setCompareValue(TimerCompare::A, 107);
        }
        break;
        
        case Note::E: {
            _timer.setCompareValue(TimerCompare::A, 95);
        }
        break;
        
    }

    switch(octave) 
    {
        case Octave::LOW: {
            _timer.setPrescalar(TimerPrescalar::TWO_FIFTY_SIX);
            break;
        }
        case Octave::MEDIUM: {
            _timer.setPrescalar(TimerPrescalar::ONE_TWENTY_EIGHT);
            break;
        }
        case Octave::HIGH: {
            _timer.setPrescalar(TimerPrescalar::SIXTY_FOUR);
            break;
        }
    }
    _timer.start();
    
}