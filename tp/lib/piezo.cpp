#include "piezo.h"

Piezo::Piezo(Timer2 timer) 
    : _timer(Timer2())
{

}

void Piezo::play(Note note, Octave octave) {
    _timer.setCompareValue(TimerCompare::A, 142);
}