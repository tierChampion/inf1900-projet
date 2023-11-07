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
    static const uint8_t MAX_LOOP_INSTRUCTIONS = (uint8_t)256;

public:
    Interpreter();
    void interpreteByteCode(uint16_t adress);
    uint8_t executeCommand(uint8_t index, uint8_t *command);

private:
    uint8_t interpreteLoop(uint8_t index);

private:
    Navigation _navigation;
    Piezo _piezo;
    Led _led;
    LoopManager _loopManager;
    Memoire24CXXX _eeprom;
    bool _executeEnable;
    static uint8_t _commands[MAX_LOOP_INSTRUCTIONS];
    uint8_t _operand;
    Instruction _instruction;
};
#endif