#include "loop_manager.h"

#include "instructions.h"

LoopManager::LoopManager() : _returnAddress(0), _counter(0) {}

void LoopManager::startLoop(uint8_t index, uint8_t counter)
{
    _counter = counter;

    _returnAddress = index + INSTRUCTION_SIZE;
}

uint16_t LoopManager::stopLoop(uint8_t index)
{
    _counter--;

    if (_counter == 0)
    {
        return index + INSTRUCTION_SIZE;
    }
    else
    {
        return _returnAddress;
    }
}