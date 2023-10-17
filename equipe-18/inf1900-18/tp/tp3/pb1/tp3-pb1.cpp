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
#include <util/delay.h>

const uint8_t LED_RED = (1 << PA0);
const uint8_t LED_GREEN = (1 << PA1);
const uint32_t ATTENUATION_DURATION_US = 3000000;
const uint16_t PULSE_LENGTH_US = 1000;
const uint16_t TOTAL_PULSES = ATTENUATION_DURATION_US / PULSE_LENGTH_US;
const uint16_t DUTY_ADJUST = 1;
const uint8_t CYCLE_PER_ADJUST = 3;
const uint8_t DELAY_RESOLUTION = 10;

uint16_t a = PULSE_LENGTH_US;
uint16_t c = 0;

void openGreenLed()
{
    PORTA &= ~LED_RED;
    PORTA |= LED_GREEN;
}

void closeGreenLed()
{
    PORTA &= ~LED_RED;
    PORTA &= ~LED_GREEN;
}

void myDelayUs(uint16_t us)
{
    for (uint16_t i = 0; i < us / DELAY_RESOLUTION; i++)
    {
        _delay_us(DELAY_RESOLUTION);
    }
}

void generatePwmPulse()
{
    openGreenLed();
    myDelayUs(a);

    closeGreenLed();
    myDelayUs(c);
}

void adjustDutyCycle(uint16_t currentPulse)
{
    if (currentPulse % CYCLE_PER_ADJUST == 0)
    {
        a -= DUTY_ADJUST;
        c += DUTY_ADJUST;
    }
}

int main()
{
    DDRA |= LED_RED | LED_GREEN;

    for (uint16_t i = 0; i < TOTAL_PULSES; i++)
    {
        generatePwmPulse();
        adjustDutyCycle(i);
    }
}
