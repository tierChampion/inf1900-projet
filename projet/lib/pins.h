#ifndef PINS_H
#define PINS_H

#include "commons.h"

enum class Port
{
    A,
    B,
    C,
    D
};

class ReadPin
{
public:
    ReadPin();
    ReadPin(Port pinPort, uint8_t position);
    uint8_t read() const;
    uint8_t getPosition() const;

private:
    Register _mode;
    Register _pin;
    uint8_t _position;
};

class WritePin
{
public:
    WritePin();
    WritePin(Port pinPort, uint8_t position);
    void set();
    void clear();
    uint8_t getPosition() const;

private:
    Register _mode;
    Register _port;
    uint8_t _position;
};

#endif
