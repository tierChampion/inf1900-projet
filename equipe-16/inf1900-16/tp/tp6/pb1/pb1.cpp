///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb1.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
///
///   Description du programme:Le programme crée un compteur qui incrémente quand le bouton est enfoncé
///                            et allume une lumiere verte lorsque relaché ou lorsqu'il équivaut à 120.
///                            Apres deux secondes d'inactivité la rouge clignote a un rythme de 2 fois
///                            par seconde pendant la moitié du compteur. Enfin, la lumière devient verte
///                            pendant 1 seconde, puis s'éteint, ramenant le robot à son état initial.
/// La borne négative de la DEL est reliée à la broche B0 et la borne positive de la DEL est reliée à la broche B1.
/// B1----> borne +
/// B0----> borne -
/// La broche D2 est en entree et est relie au bouton poussoir
/// Les broches B0 et B1 en sortie.
///
/* La table des etats :
+-------------+---------+--------------+
| ETAT ACTUEL |    ETAT FUTURE         |
+-------------+---------+--------------+
|             | FMontant | FDescendant | FMontant: Interrupteur de la broche D2 au front montant
+-------------+---------+--------------+ FDescendant:Interrupteur de la broche D2 au front descendant
| WAIT        | EXECUTE | WAIT         |
| EXECUTE     | WAIT    | WAIT         |
+-------------+---------+--------------+
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>

const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 30;
const uint8_t LED_RED = (1 << PB1);
const uint8_t LED_GREEN = (1 << PB0);
const uint8_t MASK_LED = LED_GREEN | LED_RED;
const uint8_t COUNTER_MAX = 120;
const uint16_t ONE_TENTH_SECOND = 781;
const uint8_t TIME_BLINKING_RED = 250;
const uint16_t TIME_LIGHTNING_GREEN_1 = 500;
const uint16_t TIME_LIGHTNING_GREEN_2 = 1000;
const uint16_t TIME_LED_OFF = 2000;

enum class SystemState
{
    WAIT,
    EXECUTE

};
enum Color
{
    RED,
    GREEN,
    BLACK
};

volatile SystemState state = SystemState::WAIT;
volatile uint8_t counter = 0;
uint8_t readButton()
{
    return PIND & (1 << PD2);
}

ISR(TIMER1_COMPA_vect)
{
    if (!readButton())
        counter++;
    if (counter == COUNTER_MAX)
        state = SystemState::EXECUTE;
}

ISR(INT0_vect)
{
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t read = readButton();
    if (read)
        state = SystemState::EXECUTE;
}

void initTimer(uint16_t duree)
{
    TCNT1 = 0;
    OCR1A = duree;
    TCCR1A |= 0;
    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
    TCCR1C |= 0;
    TIMSK1 |= (1 << OCIE1A);
}

void initInterrupt()
{
    cli();
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01) | (1 << ISC00);
    sei();
}

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
    case Color::BLACK:
    default:
        break;
    }
}

void executeState()
{
    switch (state)
    {
    case SystemState::WAIT:
        break;
    case SystemState::EXECUTE:
        cli();
        setLedColor(Color::GREEN);
        _delay_ms(TIME_LIGHTNING_GREEN_1);
        setLedColor(Color::BLACK);
        _delay_ms(TIME_LED_OFF);
        for (uint8_t i = 0; i < (counter / 2); i++)
        {
            setLedColor(Color::RED);
            _delay_ms(TIME_BLINKING_RED);
            setLedColor(Color::BLACK);
            _delay_ms(TIME_BLINKING_RED);
        }
        setLedColor(Color::GREEN);
        _delay_ms(TIME_LIGHTNING_GREEN_2);
        setLedColor(Color::BLACK);
        state = SystemState::WAIT;
        counter = (uint8_t)0;
        sei();
        break;
    default:
        break;
    }
}

int main()
{
    DDRD &= ~(MASK_INTERRUPT);
    DDRB |= (MASK_LED);
    initInterrupt();
    initTimer(ONE_TENTH_SECOND);
    while (true)
    {
        executeState();
    }
}
