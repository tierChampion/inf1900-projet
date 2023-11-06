#include "led.h"
#include "debug.h"
#include "memoire_24.h"

int main()
{
    Led led(Port::A, PA0, PA1);

    Memoire24CXXX memory = Memoire24CXXX();

    led.setColor(LedColor::RED);

    uint16_t length;
    uint8_t data[Comm::MAX_RECEIVE_SIZE];
    Comm::receiveData(data, &length);

    bool isTransferStarted = false;
    bool isTransferDone = false;

    uint8_t address = 0;

    for (uint8_t i = 0; i < length; i++)
    {
        PRINT(data[i]);
        if (data[i] == 0x00)
        {
            isTransferStarted = true;
        }
        else if (data[i] == 0xFF)
        {
            isTransferDone = true;
        }
        if (isTransferStarted && !isTransferDone)
        {
            memory.ecriture(address, data[i]);
            address++;
        }
    }

#ifdef DEBUG
    memory.lecture(0, data, length);

    for (uint8_t i = 0; i < length; i++)
    {
        PRINT(data[i]);
    }
#endif

    led.setColor(LedColor::GREEN);

    return 0;
}