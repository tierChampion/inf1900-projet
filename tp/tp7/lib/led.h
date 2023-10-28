#ifndef LED_H
#define LED_H

#include <avr/io.h>

#include "commons.h"

enum class LedColor
{
    OFF,
    GREEN,
    RED
};

class Led
{
public:
    Led(Register port, Register mode, uint8_t pinGreen, uint8_t pinRed);
    void setColor(LedColor color);

private:
    void setOff();
    void setGreen();
    void setRed();

private:
    Register _port;
    Register _mode;
    uint8_t _pinGreen;
    uint8_t _pinRed;
};
#endif