#ifndef COMM_H
#define COMM_H

#include <avr/io.h>

class Comm
{
public:
    static const uint8_t MAX_RECEIVE_SIZE = 255;

    static bool _isInitialised;

private:
    static Comm _comm;

public:
    static void initialiseComm();
    static void transmitData(const uint8_t *data, uint8_t length);
    static void receiveData(uint8_t *data, uint8_t *length);

private:
    Comm();
    static uint8_t singleReceive();
};

#endif
