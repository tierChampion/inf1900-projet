#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <avr/io.h>
#include <util/delay.h>

#include "navigation.h"
#include "led.h"
#include "piezo.h"
#include "loopManager.h"
#include "comm.h"
#include "memoire_24.h"
#include "instructions.h"

class Interpreter
{
public:
    Interpreter();
    void startInterpreting(uint8_t adress);
    uint8_t executeCommand(uint8_t adress, uint8_t *command);

private:
    Navigation _navigation;
    Piezo _piezo;
    Led _led;
    LoopManager _loopManager;
    Memoire24CXXX _eeprom;
    bool _executeEnable;
    uint8_t _operand;
    Instruction _instruction;
};
#endif