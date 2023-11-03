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
}

enum class Register 
{
    LOW,
    MEDIUM,
    HIGH
}

class Piezo
{
public:
    Piezo(Timer2 timer);
    void play(Note note, Register register);
    void stop();
private:
    
}

#endif