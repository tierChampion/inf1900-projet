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

void initialisationUART(void)
{

    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0
    UCSR0A = (1 << TXC0); // USART Transmit complete
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable recept and transm

    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C = (1 << UCSZ01) | ((1 << UCSZ00));
}

void transmissionUART(uint8_t donnee)
{

    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
    {}
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

int main()
{
    char mots[21] = "Le robot en INF1900\n";

    uint8_t i, j;

    initialisationUART();

    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < 20; j++)
        {
            transmissionUART(mots[j]);
        }
    }
}
