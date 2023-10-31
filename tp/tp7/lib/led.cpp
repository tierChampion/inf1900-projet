#include "led.h"

Led::Led(Port port, uint8_t greenPosition, uint8_t redPosition)
    : _greenPin(WritePin(port, greenPosition)), _redPin(WritePin(port, redPosition))
{
}

void Led::setColor(LedColor color)
{
    setOff();
    switch (color)
    {
        case LedColor::GREEN:
            setGreen();
            break;
        case LedColor::RED:
            setRed();
            break;
        default:
            break;
    }
}

void Led::setOff()
{
    _greenPin.clear();
    _redPin.clear();
}

void Led::setGreen()
{
    _greenPin.set();
}

void Led::setRed()
{
    _redPin.set();
}