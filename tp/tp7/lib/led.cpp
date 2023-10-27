
#include "led.h"
Led::Led(Register port, Register mode, uint8_t pinGreen, uint8_t pinRed)
    : _port(port), _mode(mode), _pinGreen(pinGreen), _pinRed(pinRed)
{
    *mode |= (1 << pinGreen) | (1 << pinRed);
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
    *_port &= ~((1 << _pinGreen) | (1 << _pinRed));
}
void Led::setGreen()
{
    *_port |= (1 << _pinGreen);
}
void Led::setRed()
{
    *_port |= (1 << _pinRed);
}