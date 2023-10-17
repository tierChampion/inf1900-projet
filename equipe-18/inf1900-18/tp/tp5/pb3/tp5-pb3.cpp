/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: A l'aide d'une onde PWM, faire tourner le moteur. Pendant les 10 premieres secondes, la frequence de la
//      PWM est de 60 Hz et pendant les 10 dernieres secondes, la frequence est de est de 400 Hz.
//
// Entrees/Sorties: La broche D2 est en entree et est utilisee pour la lecture du bouton-poussoir.
//      Les broches A0 et A1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche A0 est connectee au port positif de la DEL et la broche A1 est connectee
//      au port negatif de la DEL.
// /////////////////

#define F_CPU 8000000UL

#include <avr/io.h>
#include <string.h>

#include "memoire_24.h"

const uint16_t ADDRESS_START = 0x0000;

void initialisationUART(void)
{

    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0
    UCSR0A = (1 << TXC0);                 // USART Transmit complete
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable recept and transm

    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C = (1 << UCSZ01) | ((1 << UCSZ00));
}

void transmissionUART(uint8_t donnee)
{

    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

int main()
{

    Memoire24CXXX misterMemory;

    const char *MESSAGE = "Do not be afraid of those who kill the body but cannot kill the soul. Rather, be afraid of the One who can destroy both soul and body in hell - Matthew 10:28\n";
    initialisationUART();

    char ecriture[strlen(MESSAGE)];

    for (uint8_t i = 0; i < strlen(MESSAGE); i++)
    {
        ecriture[i] = MESSAGE[i];
    }

    misterMemory.ecriture(ADDRESS_START,
                          (uint8_t *)ecriture, strlen(MESSAGE));

    uint8_t lecture;

    for (uint8_t i = 0; i < strlen(MESSAGE); i++)
    {
        misterMemory.lecture(ADDRESS_START + i, &lecture);
        transmissionUART(lecture);
    }
}