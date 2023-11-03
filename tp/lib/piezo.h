#ifndef PIEZO_H
#define PIEZO_H

#include "timer2.h"

enum class Note
{
    A,
    ASHARP,
    BFLAT,
    B,
    C,
    CSHARP,
    DFLAT,
    D,
    DSHARP,
    EFLAT,
    E,
    F,
    FSHARP,
    GFLAT,
    G,
    GSHARP,
    AFLAT
};

enum class Octave 
{
    LOW,
    MEDIUM,
    HIGH
};

class Piezo
{
public:

    Piezo(Timer2 timer);
    void play(Note note, Octave octave);
    void stop();

private:
    
    Timer2 _timer;

};

#endif