///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb3.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>
#include <string.h>
#include "memoire_24.h"
const uint16_t ADRESSE_INIT = 0x0000;

void initialisationUART(void)
{
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A |= 0;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}
void transmissionUART(uint8_t donnee)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = donnee;
}
int main()
{
    initialisationUART();

    DDRA |= (1 << PA0 | 1 << PA1);
    Memoire24CXXX memoirePrincipale;
    char donneeTransmis[] = "Pourquoi faire un simple transfert de characteres est complique?";
    uint8_t tailleDuString = strlen(donneeTransmis);
    char donneeRecues[tailleDuString];
    memoirePrincipale.ecriture(ADRESSE_INIT, (uint8_t *)donneeTransmis, tailleDuString);
    memoirePrincipale.lecture(ADRESSE_INIT, (uint8_t *)donneeRecues, tailleDuString);
    uint8_t j;
    for (j = 0; j < tailleDuString; j++)
    {
        transmissionUART(donneeRecues[j]);
    }
}
