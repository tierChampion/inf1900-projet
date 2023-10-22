#include "externalMemory.h"

ExternalMemory::ExternalMemory() : 
    _protocolManager() {}

void ExternalMemory::write(const uint16_t addressStart, const uint8_t *input, uint8_t length)
{
    uint8_t writing[length];

    for (uint8_t i = 0; i < length; i++) {
        writing[i] = input[i];
    }

    _protocolManager.ecriture(addressStart, writing, length);
}

void ExternalMemory::read(const uint16_t addressStart, uint8_t* output, uint8_t length)
{
    _protocolManager.lecture(addressStart, output, length);
}
