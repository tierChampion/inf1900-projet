#include "led.h"
#include "debug.h"
#include "memoire_24.h"
#include "instructions.h"

void writeBytecodeToMemory(Memoire24CXXX memory, uint8_t* data, uint16_t length) 
{
    bool isTransferStarted = false;
    bool isTransferDone = false;

    uint16_t address = 0;

    PRINT("INSTRUCTIONS: ");

    for (uint16_t i = 0; i < length && !isTransferDone; i += INSTRUCTION_SIZE)
    {
        PRINT(data[i]);
        if (static_cast<Instruction>(data[i]) == Instruction::DBT)
        {
            isTransferStarted = true;
        }
        else if (static_cast<Instruction>(data[i]) == Instruction::FIN)
        {
            isTransferDone = true;
        }
        if (isTransferStarted)
        {
            memory.ecriture(address, &data[i], INSTRUCTION_SIZE);
            address += INSTRUCTION_SIZE;
        }
    }
}

void verifySuccess(Memoire24CXXX memory, uint8_t* data, uint16_t length) {

    PRINT("PROGRAM IN MEMORY:");
    memory.lecture(0, data, length);

    for (uint8_t i = 0; i < length; i++)
    {
        PRINT(data[i]);
    }
}

int main()
{
    Led led(Port::A, PA0, PA1);

    Memoire24CXXX memory = Memoire24CXXX();

    led.setColor(LedColor::RED);

    uint16_t length;
    uint8_t data[Comm::MAX_RECEIVE_SIZE];
    Comm::receiveData(data, &length);

    writeBytecodeToMemory(memory, data, length);

#ifdef DEBUG
    verifySuccess(memory, data, length);
#endif

    led.setColor(LedColor::GREEN);

    return 0;
}