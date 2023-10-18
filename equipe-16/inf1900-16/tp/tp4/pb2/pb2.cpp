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
#include <avr/interrupt.h>
const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 30;
const uint8_t LED_RED = (1 << PA1);
const uint8_t LED_GREEN = (1 << PA0);
const uint8_t MASK_LED = LED_GREEN | LED_RED;

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;
bool readButton()
{
    return PIND & (1 << PD2);
}
ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = 1;
}

ISR(INT0_vect)
{
    // anti-rebond
    uint8_t lecture1 = readButton();
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = readButton();
    gBoutonPoussoir = (lecture1 && lecture2);
}
void partirMinuterie(uint16_t duree)
{
    gMinuterieExpiree = 0;
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCNT1 = 0;
    OCR1A = duree;
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
    TCCR1C = 0;
    TIMSK1 = (1 << OCIE1A);
}
void initInterrupt()
{
    cli();
    EIMSK = (1 << INT0);
    EICRA = (1 << ISC00);
    sei();
}
void switchLedToGreen()
{
    PORTA &= ~(LED_RED);
    PORTA |= (LED_GREEN);
}

void switchLedToRed()
{
    PORTA &= ~(LED_GREEN);
    PORTA |= (LED_RED);
}
int main()
{
    DDRD &= ~(MASK_INTERRUPT);
    DDRA |= (MASK_LED);
    const uint16_t duree = 7813;
    _delay_ms(10000); // attendre 10s
    switchLedToRed();
    _delay_ms(100); // attendre 1/10s
    PORTA &= ~MASK_LED;
    initInterrupt();
    partirMinuterie(duree);
    do
    {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    } while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0);
    // Une interruption s'est produite. Arrêter toute
    // forme d'interruption. Une seule réponse suffit.
    cli();
    if (gMinuterieExpiree == 1)
        switchLedToRed();
    else
        switchLedToGreen();
}
