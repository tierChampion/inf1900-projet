#include "comm.h"

Comm::Comm()
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

void Comm::transmitData(const uint8_t *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        /* Wait for empty transmit buffer */
        while (!(UCSR0A & (1 << UDRE0)))
        {
        }
        /* Put data into buffer, sends the data */
        UDR0 = data[i];
    }
}