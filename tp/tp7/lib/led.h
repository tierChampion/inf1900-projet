#ifndef LED_H
#define LED_H

#include <avr/io.h>

#include "pins.h"

enum class LedColor
{
    OFF,
    GREEN,
    RED
};

class Led
{
public:
    Led(Port port, uint8_t greenPosition, uint8_t redPosition);
    void setColor(LedColor color);

private:
    void setOff();
    void setGreen();
    void setRed();

private:

    WritePin _greenPin;
    WritePin _redPin;
};
#endif