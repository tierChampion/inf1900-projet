#ifndef LED_H
#define LED_H
#include <avr/io.h>
typedef volatile uint8_t *Register;
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

public:
    void setOff();
    void setGreen();
    void setRed();

private:
    volatile uint8_t *_port;
    volatile uint8_t *_mode;
    uint8_t _pinGreen;
    uint8_t _pinRed;
};
#endif