#ifndef COMM_H
#define COMM_H

#include <avr/io.h>

class Comm
{
public:
    static const uint16_t MAX_RECEIVE_SIZE = 512;

private:
    static Comm _comm;
    static bool _isInitialised;

public:
    static void initialiseComm();
    static void transmitData(const uint8_t *data, uint16_t length);
    static void receiveData(uint8_t *data, uint16_t *length);

private:
    Comm();
    static uint8_t singleReceive();
};

#endif
