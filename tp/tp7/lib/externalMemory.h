#ifndef EXTERNAL_MEM_H
#define EXTERNAL_MEM_H

#include <avr/io.h>
#include <string.h>

#include "memoire_24.h"

class ExternalMemory
{
public:

    ExternalMemory();

    void write(const uint16_t addressStart, const uint8_t* input, const uint8_t length);
    void read(const uint16_t addressStart, uint8_t* output, const uint8_t length);

private:

    Memoire24CXXX _protocolManager;
};

#endif