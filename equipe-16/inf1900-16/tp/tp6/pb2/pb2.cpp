///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb1.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
///
///   Description du programme:Le programme sert à évaluer l'intensité lumineuse grâce à une photorésistance. Il fait
///                            en sorte que la led s'allume en vert en cachant la photorésistance (si la lumière est
///                            basse), en ambré si la lumière est ambiante (la lumiere a un bon niveau) et en rouge
///                            si la photoresistance est sous une lampe de poche (la lumiere est trop forte).
///
/// La borne négative de la DEL est reliée à la broche B0 et la borne positive de la DEL est reliée à la broche B1.
/// B1----> borne +
/// B0----> borne -
/// Les broches B0 et B1 en sortie.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include "can.h"

const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t MASK_A0 = (1 << PA0);
const uint8_t MASK_A1 = (1 << PA1);
const uint8_t LED_RED = (1 << PB1);
const uint8_t LED_GREEN = (1 << PB0);
const uint8_t MASK_LED = LED_GREEN | LED_RED;
const uint8_t DEBOUNCE_DELAY = 10;
const uint8_t TIME_SWITCHING_AMBRE = 5;
const uint8_t ROOM_LIGHT_INTENSITY_MIN = 200;
const uint8_t ROOM_LIGHT_INTENSITY_MAX = 240;
const uint8_t convertorAnalogicNumericPosition = PA0;
enum Color
{
    RED,
    GREEN,
    BLACK,
    AMBER
};
class can convertorAnalogicNumeric;
uint8_t lightIntensity;

void setLedColor(Color color)
{
    PORTB &= ~(MASK_LED);
    switch (color)
    {
    case Color::RED:
        PORTB |= (LED_RED);
        break;
    case Color::GREEN:
        PORTB |= (LED_GREEN);
        break;
    case Color::AMBER:
        setLedColor(Color::GREEN);
        _delay_ms(TIME_SWITCHING_AMBRE);
        setLedColor(Color::RED);
        _delay_ms(TIME_SWITCHING_AMBRE);
        break;
    case Color::BLACK:
    default:
        break;
    }
}

void execute()
{
    lightIntensity = (convertorAnalogicNumeric.lecture(convertorAnalogicNumericPosition) >> 2);
    if (lightIntensity < ROOM_LIGHT_INTENSITY_MIN)
    {
        setLedColor(Color::GREEN);
    }
    else if (lightIntensity > ROOM_LIGHT_INTENSITY_MAX)
    {
        setLedColor(Color::RED);
    }
    else
    {
        setLedColor(Color::AMBER);
    }
}

int main()
{
    DDRB |= (MASK_LED);
    DDRA &= ~(MASK_A0);
    PORTA &= ~(MASK_A1);
    while (true)
    {
        execute();
    }
}