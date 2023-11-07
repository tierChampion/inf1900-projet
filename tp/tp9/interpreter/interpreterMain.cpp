#include <interpreter.h>
int main()
{
    Led led(Port::A, PA0, PA1);
    led.setColor(LedColor::GREEN);
    _delay_ms(1000);
    led.setColor(LedColor::RED);
    _delay_ms(1000);
    led.setColor(LedColor::OFF);

    Interpreter interpreter{};
    interpreter.interpreteByteCode(0x00);

    led.setColor(LedColor::GREEN);
    _delay_ms(1000);
    led.setColor(LedColor::RED);
    _delay_ms(1000);
    led.setColor(LedColor::OFF);
}