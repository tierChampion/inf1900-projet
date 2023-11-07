#include "interpreter.h"
#include "button.h"

int main()
{

    // Led led(Port::A, PA0, PA1);
    // led.setColor(LedColor::GREEN);
    // _delay_ms(1000);
    // led.setColor(LedColor::RED);
    // _delay_ms(1000);
    // led.setColor(LedColor::OFF);

    Interpreter interpreter = Interpreter();
    interpreter.interpreteLoop();

    // led.setColor(LedColor::GREEN);
    // _delay_ms(1000);
    // led.setColor(LedColor::RED);
    // _delay_ms(1000);
    // led.setColor(LedColor::OFF);
}
