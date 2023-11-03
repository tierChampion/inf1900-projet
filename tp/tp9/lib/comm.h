#ifndef COMM_H
#define COMM_H

#include <avr/io.h>

class Comm
{
public:
    Comm();

    static void transmitData(const uint8_t *data, uint8_t length);
};

#endif
