#include "loopManager.h"

LoopManager::LoopManager() : _returnAddress(0), _counter(0) {}

void LoopManager::startLoop(uint16_t address, uint8_t counter)
{
    _counter = counter;

    _returnAddress = address + 2;
}

uint16_t LoopManager::stopLoop(uint16_t address)
{
    _counter--;

    if (_counter == 0)
    {
        return address + 2;
    }
    else
    {
        return _returnAddress;
    }
}