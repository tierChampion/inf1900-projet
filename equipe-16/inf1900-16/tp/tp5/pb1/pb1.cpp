//////////////////////////////////////////////////////////////////
///  \file   pb1.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
//////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "memoire_24.h"
const uint16_t ADRESSE_INIT = 0x0000;
int main()
{
    DDRA |= (1 << PA0 | 1 << PA1);
    Memoire24CXXX memoirePrincipale;
    char donneeTransmis[] = " *P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R**A*L*\0";
    uint8_t tailleDuString = strlen(donneeTransmis);
    memoirePrincipale.ecriture(ADRESSE_INIT, (uint8_t *)donneeTransmis, tailleDuString);
    char donneeRecues[tailleDuString];
    memoirePrincipale.lecture(ADRESSE_INIT, (uint8_t *)donneeRecues, tailleDuString);
    PORTA |= strcmp(donneeRecues, donneeTransmis) ? (1 << PA0) : (1 << PA1);
}