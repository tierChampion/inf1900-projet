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
const uint8_t MASK_A0 = (1 << PA0);
const uint8_t MASK_A1 = (1 << PA1);
const uint8_t numCycleDelayLoop2 = 4;

void avant()
{
    PORTA |= MASK_A0;
}
// std::function fontion= [](){};
/*void arriere()
{
    PORTA = 0x02;
    PORTB = 0x80;
}*/

void off()
{
    PORTA &= ~(MASK_A0);
}
void avantAFHz(uint16_t frequence)
{
    uint16_t a = 1;
    uint16_t c = (F_CPU / (frequence * numCycleDelayLoop2)) + 1;
    uint16_t rapport = (c - 1) / 4;

    for (int i = 0; i < 5; i++)
    {

        for (int j = (2 * frequence); j >= 1; j--)
        {
            avant();
            _delay_loop_2(a);
            off();
            _delay_loop_2(c);
        }
        a += rapport;
        c -= rapport;
    }
}

/*void arriereOff()
{
    // uint8_t a = numCycleDelayLoop2 / (F_CPU);
    uint16_t a = 6000;
    uint16_t c = 1;
    // for (int i = 3; i >= 1; i--)
    //{
    for (int j = 2999; j >= 1; j--)
    {
        arriere();
        _delay_loop_2(a / 3);
        off();
        _delay_loop_2(c / 3);
        a -= 2;
        c = 6000 - a;
    }
}*/

int main()
{
    DDRA |= (MASK_A0 | MASK_A1);
    PORTA &= ~MASK_A1;
    while (true)
    {
        avantAFHz(60);
        avantAFHz(400);
    }
}