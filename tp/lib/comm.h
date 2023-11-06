#ifndef COMM_H
#define COMM_H

#include <avr/io.h>

class Comm
{
public:

    static const uint8_t MAX_RECEIVE_SIZE = 255;

public:
    Comm();

    static void transmitData(const uint8_t *data, uint8_t length);
    static void receiveData(uint8_t* data, uint16_t *length);

private:
    static uint8_t singleReceive();
};

#endif
