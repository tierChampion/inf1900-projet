/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: Utilisation du Convertisseur Analogique Numerique (CAN) pour lire la
//      luminosite sur une photoresistance. Un circuit sur le breadboard est utilise
//      pour connecter la photoresistance aux broches de la carte mere. Lorsque la luminosite
//      mesure est faible, la DEL allume verte, lorsqu'elle est moyenne, la DEL allume ambre
//      et lorsqu'elle est forte, la DEL allume rouge.
//
// Entrees/Sorties: La broche A0 est en entree et est utilisee pour le CAN.
//      Les broches B0 et B1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche B0 est connectee au port positif de la DEL et la broche B1 est connectee
//      au port negatif de la DEL.
// /////////////////

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can.h"

const uint8_t LED_RED = (1 << PB0);
const uint8_t LED_GREEN = (1 << PB1);
const uint8_t INPUT_PIN = PA0;
const uint8_t CAN_INPUT = (1 << INPUT_PIN);
const uint8_t PRECISION_LOSS = 2;
const uint8_t CAN_INPUT_MASK = 0xFF;

const uint8_t LOW_LIGHT = 0xD3;
const uint8_t MEDIUM_LIGHT = 0xF0;

volatile uint8_t gAmberCounter = 0;
const uint8_t AMBER_COUNTER_MAX = 8;
const uint8_t AMBER_GREEN_MAX = 6;

const uint16_t FLASHING_COMPARE_VALUE = 120;

enum class LedColor
{
    RED,
    GREEN,
    AMBER
};

void openGreenLed()
{
    PORTB &= ~LED_RED;
    PORTB |= LED_GREEN;
}

void openRedLed()
{
    PORTB &= ~LED_GREEN;
    PORTB |= LED_RED;
}

void openAmberLed()
{
    if (gAmberCounter <= AMBER_GREEN_MAX)
    {
        openGreenLed();
    }
    else
    {
        openRedLed();
    }
}

void openLed(LedColor color)
{

    switch (color)
    {
        case LedColor::RED:
            openRedLed();
            break;
        case LedColor::GREEN:
            openGreenLed();
            break;
        case LedColor::AMBER:
            openAmberLed();
            break;
    }
}

void closeLed()
{
    PORTB &= ~LED_RED;
    PORTB &= ~LED_GREEN;
}

ISR(TIMER1_COMPA_vect)
{
    gAmberCounter++;
    openLed(LedColor::AMBER);
    gAmberCounter %= AMBER_COUNTER_MAX;
}

void initTimer()
{
    cli();

    // Toggle on compare match
    TCCR1A &= ~(1 << COM1A0);
    TCCR1A |= (1 << COM1A1);

    // Set Waveform Generation mode to CTC
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    // Prescalar of 0 - Timer stopped
    TCCR1B &= ~(1 << CS10);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12);

    sei();
}

bool isTimerRunning()
{
    // Check if the prescalar is 0
    return (TCCR1B & (1 << CS10 | 1 << CS11 | 1 << CS12)) != 0;
}

void startTimer(uint16_t compareValue)
{
    if (!isTimerRunning())
    {
        cli();

        // Initial value of 0
        TCNT1 = 0;

        // Set compare value A
        OCR1A = compareValue;

        // Prescalar of 8
        TCCR1B |= (1 << CS10);
        TCCR1B |= (1 << CS11);
        TCCR1B &= ~(1 << CS12);

        // Enable TIM1 Compare A interrupt
        TIMSK1 |= (1 << OCIE1A);

        sei();
    }
}

void stopTimer()
{
    if (isTimerRunning())
    {
        cli();

        // Prescalar of 0 - Timer stopped
        TCCR1B &= ~(1 << CS10);
        TCCR1B &= ~(1 << CS11);
        TCCR1B &= ~(1 << CS12);

        // Disable TIM1 Compare A interrupt
        TIMSK1 &= ~(1 << OCIE1A);
        sei();
    }
}

uint8_t proccessAnalogInput(uint16_t rawInput)
{
    return static_cast<uint8_t>((rawInput >> PRECISION_LOSS) & CAN_INPUT_MASK);
}

void initialisation()
{

    DDRA &= ~CAN_INPUT;
    DDRB |= LED_GREEN | LED_RED;

    initTimer();
}

int main()
{
    initialisation();

    can converter = can();

    while (true)
    {
        uint8_t proccessedInput = proccessAnalogInput(converter.lecture(INPUT_PIN));

        if (proccessedInput < LOW_LIGHT)
        {
            openLed(LedColor::GREEN);
            stopTimer();
        }
        else if (proccessedInput < MEDIUM_LIGHT)
        {
            startTimer(FLASHING_COMPARE_VALUE);
        }
        else
        {
            openLed(LedColor::RED);
            stopTimer();
        }
    }
}
