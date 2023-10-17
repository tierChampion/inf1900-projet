#include <led.h>

int main()
{
    Led led = Led(&PORTA, &DDRA, PA0, PA1);

    led.setOff();

    return 0;
}

