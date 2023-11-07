#include "loopManager.h"

LoopManager::LoopManager() : _returnAddress(0), _counter(0) {}

void LoopManager::startLoop(uint8_t index, uint8_t counter)
{
    _counter = counter;

    _returnAddress = index + 2;
}

uint16_t LoopManager::stopLoop(uint8_t index)
{
    _counter--;

    if (_counter == 0)
    {
        return index + 2;
    }
    else
    {
        return _returnAddress;
    }
}