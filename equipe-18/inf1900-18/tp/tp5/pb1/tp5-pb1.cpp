/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: Utilisation de la memoire externe.
//
// Entrees/Sorties: Les broches A0 et A1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche A0 est connectee au port positif de la DEL et la broche A1 est connectee
//      au port negatif de la DEL.
/////////////////

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "memoire_24.h"

const uint8_t LED_RED = (1 << PA0);
const uint8_t LED_GREEN = (1 << PA1);
const uint8_t LED_OFF = (1 << PA0) | (1 << PA1);
const uint8_t WRITE_DELAY = 5;
const uint8_t ADDRESS_START = 0x0000;
#define STRING_LENGTH 45
const char *MESSAGE = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";

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
    PORTA &= ~LED_OFF;
}

void initialisation()
{

    DDRA |= LED_RED | LED_GREEN;
}

int main()
{

    Memoire24CXXX misterMemoire;

    initialisation();

    char ecriture[STRING_LENGTH + 1];

    for (uint8_t i = 0; i < STRING_LENGTH; i++)
    {
        ecriture[i] = MESSAGE[i];
    }

    ecriture[STRING_LENGTH] = 0x00;

    misterMemoire.ecriture(ADDRESS_START, (uint8_t *)ecriture, STRING_LENGTH + 1);

    char lecture[STRING_LENGTH + 1];

    misterMemoire.lecture(ADDRESS_START, (uint8_t *)lecture, STRING_LENGTH + 1);

    bool isEqual = (strcmp(ecriture, lecture) == 0);

    if (isEqual)
    {
        openGreenLed();
    }
    else
    {
        openRedLed();
    }
}
