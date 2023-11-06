#ifndef PIEZO_H
#define PIEZO_H

#include "timer2.h"
#include "pins.h"

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
    HIGH,
    TOP
};

class Piezo
{
public:

    Piezo();
    void play(int val);
    void play(Note note, Octave octave);
    void stop();

private:
    
    Timer2 _timer;
    WritePin _timerPin;
    WritePin _groundPin;

};

#endif