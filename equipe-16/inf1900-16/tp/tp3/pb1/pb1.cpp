///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb1.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
///
/// Description du programme:
///
/// La borne négative de la DEL est reliée à la broche A0 et la borne positive de la DEL est reliée à la broche A1.
/// A1----> borne +
/// A0----> borne -
///
/// La broche D2 est en entree et est relie au boutton poussoir
/// Les broche A0 et A1 est en sortie.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay_basic.h>
const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t MASK_A = (1 << PA0) | (1 << PA1);
const uint8_t LED_RED = (1 << PA1);
const uint8_t LED_GREEN = (1 << PA0);
const double b = 1 / 1000; ////////////////////
const uint8_t numCycleDelayLoop2 = 4;

void switchToGreen()
{
    PORTA &= ~(LED_RED);
    PORTA |= (LED_GREEN);
}
// std::function fontion= [](){};
void switchToRed()
{
    PORTA &= ~(LED_GREEN);
    PORTA |= (LED_RED);
}

void switchOff()
{
    PORTA &= 0x00;
}
void switchGreenOff()
{
    // uint8_t a = numCycleDelayLoop2 / (F_CPU);
    uint16_t a = 6000;
    uint16_t c = 1;

    for (int j = 2999; j >= 1; j--)
    {
        switchToGreen();
        _delay_loop_2(a / 3);
        switchOff();
        _delay_loop_2(c / 3);
        a -= 2;
        c = 6000 - a;
    }
}

void switchRedOff()
{
    // uint8_t a = numCycleDelayLoop2 / (F_CPU);
    uint16_t a = 6000;
    uint16_t c = 1;
    // for (int i = 3; i >= 1; i--)
    //{
    for (int j = 2999; j >= 1; j--)
    {
        switchToRed();
        _delay_loop_2(a / 3);
        switchOff();
        _delay_loop_2(c / 3);
        a -= 2;
        c = 6000 - a;
    }
}

int main()
{
    DDRA |= (MASK_A);
    DDRD &= ~MASK_INTERRUPT;
    while (true)
    {
        switchToGreen();
        //_delay_ms(1000);
        switchGreenOff();
        switchToRed();
        //_delay_ms(1000);
        switchRedOff();
    }
}
