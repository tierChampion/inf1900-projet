#include "loopManager.h"

LoopManager::LoopManager() : _returnAddress(0), _counter(0), _loopStarted(false), _loopEnded(false) {}

bool LoopManager::loopStarted() const
{
    return _loopStarted;
}

bool LoopManager::loopEnded() const
{
    return _loopEnded;
}

void LoopManager::startLoop(uint8_t index, uint8_t counter)
{
    _loopStarted = true;
    _loopEnded = false;
    _counter = counter;

    _returnAddress = index + 2;
}

uint16_t LoopManager::stopLoop(uint8_t index)
{
    _counter--;

    if (_counter == 0)
    {
        _loopEnded = true;
        _loopStarted = false;
        return index + 2;
    }
    else
    {
        return _returnAddress;
    }
}