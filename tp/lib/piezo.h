#ifndef PIEZO_H
#define PIEZO_H

#include "timer2.h"

class Piezo
{
public:
    Piezo(Timer2 timer);
    void play();
    void stop();    
}

#endif