#define F_CPU 8000000UL
#include <led.h>
#include <navigation.h>
#include <util/delay.h>
int main()
{
    Navigation navigation{Pin(&DDRB, &PORTB, &PINB, PB5), Pin(&DDRB, &PORTB, &PINB, PB6)};
    while (true)
    {
        navigation.move(Direction::FORWARD, 50.0);
        _delay_ms(7000);
        navigation.move(Direction::BACKWARD, 50.0);
        _delay_ms(7000);
        navigation.turn(Direction::LEFT, 50.0);
        _delay_ms(4000);
        navigation.turn(Direction::RIGHT, 50.0);
        _delay_ms(4000);
    }
    return 0;
}
