#ifndef PIEZO_H
#define PIEZO_H

#include "timer2.h"
#include "pins.h"

enum class Note : uint8_t
{
    A = 0,
    ASHARP_BFLAT = 1,
    B = 2,
    C = 3,
    CSHARP_DFLAT = 4,
    D = 5,
    DSHARP_EFLAT = 6,
    E = 7,
    F = 8,
    FSHARP_GFLAT = 9,
    G = 10,
    GSHARP_AFLAT = 11
};

enum class Octave : uint8_t
{
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    TOP = 3
};

class Piezo
{
public:
    Piezo();
    void play(uint8_t val);
    void play(Note note, Octave octave);
    void stop();
    void successMelody();
    void playFoundPillarMelody();

private:
    Timer2 _timer;
    WritePin _timerPin;
    WritePin _groundPin;
};

#endif