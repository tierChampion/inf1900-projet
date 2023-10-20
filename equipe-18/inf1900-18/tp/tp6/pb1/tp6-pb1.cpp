/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: Utilisation d'un bouton-poussoir externe a la carte. Un circuit sur le breadboard
//      est utilise pour connecter ce bouton-poussoir aux broches de la carte mere. En premier lieu,
//      le bouton est appuye et commence une minuterie qui incrementera un compteur a une frequence de
//      10 Hz jusqu'a un maximum de 120. Si jamais le bouton est relache, la minuterie est arretee.
//      Par la suite, la DEL allume verte pour une demi seconde et est ensuite etteinte pour deux secondes.
//      La DEL va clignoter rouge a la frequence de 2 Hz pour un total de (compteur / 2) fois pour finalement
//      allumer verte pour une seconde et revenir a l'etat initiale.
//
// Entrees/Sorties: La broche D2 est en entree et est utilisee pour la lecture du bouton-poussoir externe.
//      Les broches B0 et B1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche B0 est connectee au port positif de la DEL et la broche B1 est connectee
//      au port negatif de la DEL.
//      Le VCC et le GND du port A est utilise pour alimenter le circuit sur le breadboard.
//      Le cavalier IntEN doit necessairement etre retirer de la carte pour permettre l'utilisation du
//      bouton-poussoir sur le breadboard.
//
// ╔═══════════════════╦═════════=═════════╦═════════════╗
// ║  STATE            ║   NEXT            ║     LED     ║
// ╠═══════════════════╬═══════════════════╬═════════════╣
// ║ START             ║ HALF_SECOND_GREEN ║ 00 (OFF)    ║
// ║ HALF_SECOND_GREEN ║ TWO_SECOND_OFF    ║ 01 (GREEN)  ║
// ║ TWO_SECOND_OFF    ║ FLASHING_RED      ║ 00 (OFF)    ║
// ║ FLASHING_RED      ║ ONE_SECOND_GREEN  ║ 10 (RED)    ║
// ║ ONE_SECOND_GREEN  ║ END               ║ 01 (GREEN)  ║
// ║ END               ║ END               ║ 00 (OFF)    ║
// ╚═══════════════════╩═══════=═══════════╩═════════════╝
////////////////////

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t LED_RED = (1 << PB0);
const uint8_t LED_GREEN = (1 << PB1);
const uint8_t BUTTON_MASK = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 30;
const uint8_t COUNTER_MAX = 120;
const uint16_t WAITING_COMPARE_VALUE = 12500;
const uint16_t FLASHING_COMPARE_VALUE = static_cast<uint16_t>(WAITING_COMPARE_VALUE * 2.5f);
const uint16_t HALF_SECOND_MS = 500;
const uint16_t ONE_SECOND_MS = 1000;
const uint16_t TWO_SECONDS_MS = 2000;

enum class Flashing
{
    START,
    HALF_SECOND_GREEN,
    TWO_SECOND_OFF,
    FLASHING_RED,
    ONE_SECOND_GREEN,
    END,
};

enum class LedColor {
    RED,
    GREEN,
};

volatile uint8_t gCounter = 0;
volatile uint8_t gFlashCount = 0;
volatile bool gWaiting = true;

void startTimer(uint16_t compareValue);
void stopTimer();

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
    }
}

void closeLed()
{
    PORTB &= ~LED_RED;
    PORTB &= ~LED_GREEN;
}

bool isDebouncedButtonPressed() {

    uint8_t reading1 = (PIND & BUTTON_MASK);

    _delay_ms(DEBOUNCE_DELAY);

    uint8_t reading2 = (PIND & BUTTON_MASK);

    bool isStable = (reading1 == reading2);
    bool isButtonPressed = (reading2 == 0);

    return isStable && isButtonPressed;
}

ISR(INT0_vect)
{
    bool isButtonPressed = isDebouncedButtonPressed();

    if (isButtonPressed)
    {
        startTimer(WAITING_COMPARE_VALUE);
        gCounter = 0;
    }
    else
    {
        gWaiting = false;
    }

    EIFR |= (1 << INTF0);
}

ISR(TIMER1_COMPA_vect)
{
    gCounter++;

    if (gCounter == COUNTER_MAX && gWaiting)
    {
        gWaiting = false;
    }
    else if (gCounter % 2 == 0)
    {
        gFlashCount--;
    }
}

void calculateFlashCount()
{
    gFlashCount = gCounter / 2;
    gCounter = 0;
}

void initInterrupt0()
{
    cli();

    // Generate an interruption at any edge.
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    sei();
}

void startInterrupt0()
{
    cli();

    // Enable INT0 on pin D2
    EIMSK |= (1 << INT0);

    // Clear any pending interrupts
    EIFR |= (1 << INTF0);

    sei();
}

void stopInterrupt0()
{
    cli();

    // Disable INT0 on pin D2
    EIMSK &= ~(1 << INT0);

    sei();
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

    sei();
}

void startTimer(uint16_t compareValue)
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

void stopTimer()
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

Flashing updateFlashing(const Flashing current)
{
    switch (current)
    {
        case Flashing::START:
        {
            return Flashing::HALF_SECOND_GREEN;
        }
        break;

        case Flashing::HALF_SECOND_GREEN:
        {
            _delay_ms(HALF_SECOND_MS);
            return Flashing::TWO_SECOND_OFF;
        }
        break;

        case Flashing::TWO_SECOND_OFF:
        {
            _delay_ms(TWO_SECONDS_MS);

            startTimer(FLASHING_COMPARE_VALUE);

            return Flashing::FLASHING_RED;
        }
        break;

        case Flashing::FLASHING_RED:
        {
            if (gFlashCount == 0)
            {
                return Flashing::ONE_SECOND_GREEN;
            }
            else
            {
                return Flashing::FLASHING_RED;
            }
        }
        break;

        case Flashing::ONE_SECOND_GREEN:
        {
            stopTimer();

            _delay_ms(ONE_SECOND_MS);

            return Flashing::END;
        }
        break;

        default:
        {
            return Flashing::END;
        }
    }
}

void executeFlashing(const Flashing current)
{
    switch (current)
    {
        case Flashing::HALF_SECOND_GREEN:
        {
            openLed(LedColor::GREEN);
        }
        break;

        case Flashing::FLASHING_RED:
        {
            if (gCounter % 2 == 0)
            {
                openLed(LedColor::RED);
            }
            else
            {
                closeLed();
            }
        }
        break;

        case Flashing::ONE_SECOND_GREEN:
        {
            openLed(LedColor::GREEN);
        }
        break;

        default:
        {
            closeLed();
        }
        break;
    }
}

void initialisation()
{

    DDRB |= LED_RED | LED_GREEN;
    DDRD &= ~(BUTTON_MASK);

    initTimer();
    initInterrupt0();
}

int main()
{
    initialisation();

    while (true)
    {
        gWaiting = true;
        startInterrupt0();

        while (gWaiting)
        {
        }

        stopTimer();
        stopInterrupt0();

        calculateFlashCount();

        Flashing state = Flashing::START;

        while (state != Flashing::END)
        {
            state = updateFlashing(state);
            executeFlashing(state);
        }
    }
}
