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
#include <util/delay.h>

void ajustementPWM(uint16_t duty)
{
    // Set Duty Cycle
    switch (duty)
    {
        // Set Duty Cycle
        // OC1A D5
        // OC1B D4
        case 0:
            OCR1A = 0;
            OCR1B = 0;
            break;
        case 25:
            OCR1A = 64;
            OCR1B = 64;
            break;
        case 50:
            OCR1A = 128;
            OCR1B = 128;
            break;
        case 75:
            OCR1A = 192;
            OCR1B = 192;
            break;
        case 100:
            OCR1A = 255;
            OCR1B = 255;
            break;
    }
}

void startMotors()
{
    // Initial value of 0
    TCNT1 = 0;

    // Clear on compare match
    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << COM1B1);

    // Set Waveform Generation mode to Phase Correct PWM
    TCCR1A |= (1 << WGM10);

    // Prescalar of 8
    TCCR1B |= (1 << CS11);
}

int main()
{
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTD &= ~(1 << PD6) & ~(1 << PD7);

    startMotors();

    while (true)
    {
        ajustementPWM(0);
        _delay_ms(2000);
        ajustementPWM(25);
        _delay_ms(2000);
        ajustementPWM(0);
        _delay_ms(2000);
        ajustementPWM(50);
        _delay_ms(2000);
        ajustementPWM(0);
        _delay_ms(2000);
        ajustementPWM(75);
        _delay_ms(2000);
        ajustementPWM(0);
        _delay_ms(2000);
        ajustementPWM(100);
        _delay_ms(2000);
    }
}
