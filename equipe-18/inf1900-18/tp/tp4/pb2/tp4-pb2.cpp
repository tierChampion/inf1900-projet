/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: A l'aide d'une onde PWM, graduellement fermer une LED verte pendant 3 secondes.
//
// Entrees/Sorties: La broche D2 est en entree et est utilisee pour la lecture du bouton-poussoir.
//      Les broches A0 et A1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche A0 est connectee au port positif de la DEL et la broche A1 est connectee
//      au port negatif de la DEL.
////////////////////

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const uint8_t BUTTON_MASK = (1 << PD2);
const uint8_t LED_RED = (1 << PA0);
const uint8_t LED_GREEN = (1 << PA1);
const uint8_t LED_OFF = (1 << PA0) | (1 << PA1);
const uint8_t DEBOUNCE_DELAY = 30;

const uint16_t ONE_SECOND = 8192;

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;

ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = 1;
}

ISR(INT0_vect)
{
    gBoutonPoussoir = 1;

    EIFR |= (1 << INTF0);
}

void openGreenLed()
{
    PORTA &= ~LED_RED;
    PORTA |= LED_GREEN;
}

void openRedLed()
{
    PORTA &= ~LED_GREEN;
    PORTA |= LED_RED;
}

void closeLed()
{
    PORTA &= ~LED_RED;
    PORTA &= ~LED_GREEN;
}

void initialisation()
{
    cli();

    DDRA |= LED_RED | LED_GREEN;
    DDRD &= ~BUTTON_MASK;

    // Enable int0
    EIMSK |= (1 << INT0); 
    // Rising edge
    EICRA |= (1 << ISC01) | (1 << ISC00);
 
    sei();
}

void startTimer(uint16_t duration)
{
    gMinuterieExpiree = 0;

    // Initial value of 0
    TCNT1 = 0;

    // Set compare value to duree
    OCR1A = duration;

    // Toggle on compare match
    TCCR1A |= (1 << COM1A0);
    TCCR1A &= ~(1 << COM1A1);

    // Set Waveform Generation mode to CTC
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    // Prescalar of 1024
    TCCR1B |= (1 << CS10);
    TCCR1B &= ~(1 << CS11);
    TCCR1B |= (1 << CS12);

    TCCR1C = 0;

    // Enable TIM1 Compare A interrupt
    TIMSK1 |= (1 << OCIE1A);
}

int main()
{
    _delay_ms(10000);

    initialisation();
    startTimer(ONE_SECOND);

    openRedLed();
    _delay_ms(100); // 1 / 10
    closeLed();

    do
    {

    } while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0);

    cli();

    if (gMinuterieExpiree == 1) {
        openRedLed();
    } else if (gBoutonPoussoir == 1) {
        openGreenLed();
    }
}
