#ifndef LOOP_H
#define LOOP_H

#include "commons.h"

class LoopManager
{

public:
    LoopManager();
    bool loopStarted() const;
    bool loopEnded() const;
    void startLoop(uint8_t index, uint8_t counter);
    uint16_t stopLoop(uint8_t index);

private:
    uint16_t _returnAddress;
    uint8_t _counter;
    bool _loopStarted;
    bool _loopEnded;
};

#endif