#include <led.h>
#include <navigation.h>
#include <util/delay.h>
int main()
{
    while (true)
    {
        Navigation navigation{Pin(&DDRB, &PORTB, PB5), Pin(&DDRB, &PORTB, PB6)};
        navigation.go(50);
        _delay_ms(2000);
        navigation.go(-50);
        _delay_ms(2000);
        navigation.turn(Side::LEFT, 50);
        _delay_ms(1000);
        navigation.turn(Side::RIGHT, 50);
        _delay_ms(1000);
    }
    return 0;
}
