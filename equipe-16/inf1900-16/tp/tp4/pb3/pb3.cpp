///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb3.cpp
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
#include <avr/interrupt.h>

const uint8_t WHEEL_RIGHT_D = (1 << PD6);
const uint8_t WHEEL_LEFT_D = (1 << PD7);
const uint8_t WHEEL_RIGHT_E = (1 << PD6);
const uint8_t WHEEL_LEFT_E = (1 << PD7);

void adjustmentPwm(uint16_t duree)
{
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)
    TCNT1 = 0;
    OCR1A = duree;
    OCR1B = duree;
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << CS11);
    TCCR1C = 0;
}

int main()
{
    DDRD |= (WHEEL_LEFT_D | WHEEL_RIGHT_D | (1 << PD4) | (1 << PD5));
    PORTD &= ~(WHEEL_LEFT_D | WHEEL_RIGHT_D);
    adjustmentPwm(1);
    _delay_ms(2000);
    adjustmentPwm(64);
    _delay_ms(2000);
    adjustmentPwm(128);
    _delay_ms(2000);
    adjustmentPwm(192);
    _delay_ms(2000);
    adjustmentPwm(255);
    _delay_ms(2000);
}