#include "led.h"

Led::Led(Register port, Register mode, uint8_t pinGreen, uint8_t pinRed) 
    : _port(port), _mode(mode), _pinGreen(pinGreen), _pinRed(pinRed)
{
     *mode |= (1 << pinGreen) | (1 << pinRed);
}

void Led::setOff() {
    *_port &= (1 << _pinRed) | (1 << _pinRed);
}

void Led::setGreen() {
    setOff();
    *_port |= (1 << _pinGreen);
}

void Led::setRed() {
    setOff();
    *_port |= (1 << _pinRed);
}