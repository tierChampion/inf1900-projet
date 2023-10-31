#include "navigation.h"
#include "button.h"
#include "led.h"
#include "pins.h"

int main()
{
    Navigation nav = Navigation();

    Button button = Button(GeneralInterruptType::INT_0, true);

    Led led = Led(Port::A, PA0, PA1);


    return 0;
}