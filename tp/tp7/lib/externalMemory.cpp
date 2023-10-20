#include "externalMemory.h"

ExternalMemory::ExternalMemory() : 
    _protocolManager() {}

void ExternalMemory::write(const uint16_t addressStart, const char *input, uint8_t length)
{
    char writing[length];

    for (uint8_t i = 0; i < length; i++) {
        writing[i] = input[i];
    }

    _protocolManager.ecriture(addressStart, (uint8_t*)writing, length);
}

void ExternalMemory::read(const uint16_t addressStart, char* output, uint8_t length)
{
    _protocolManager.lecture(addressStart, (uint8_t*)output, length);
}
