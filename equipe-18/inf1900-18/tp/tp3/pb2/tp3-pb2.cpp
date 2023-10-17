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

enum class InputState
{
    PRESSED,
    RELEASED
};

const uint8_t BUTTON_MASK = (1 << PD2);
const uint8_t FORWARD = (1 << PA0);
const uint8_t BACKWARD = (1 << PA1);
const uint8_t FORWARD_SECOND_MOTOR = (1 << PA2);
const uint8_t BACKWARD_SECOND_MOTOR = (1 << PA3);
const uint8_t DEBOUNCE_DELAY = 10;
const uint16_t LIGHT_DELAY = 3000;
const uint32_t PHASE_DURATION_US = 10000000;
const uint8_t CHANGE_PER_PHASE = 5;
const uint16_t PULSE_LENGTH_US_SIXTY = PHASE_DURATION_US / 600;
const uint16_t PULSE_COUNT_SIXTY = PHASE_DURATION_US / PULSE_LENGTH_US_SIXTY;
const uint16_t DUTY_ADJUST_SIXTY = PULSE_LENGTH_US_SIXTY / CHANGE_PER_PHASE;
const uint16_t CHANGE_LENGTH_SIXTY = PULSE_COUNT_SIXTY / CHANGE_PER_PHASE;
const uint16_t PULSE_LENGTH_US_FOURHUNDRED = PHASE_DURATION_US / 4000;
const uint16_t PULSE_COUNT_FOURHUNDRED = PHASE_DURATION_US / PULSE_LENGTH_US_FOURHUNDRED;
const uint16_t DUTY_ADJUST_FOURHUNDRED = PULSE_LENGTH_US_FOURHUNDRED / CHANGE_PER_PHASE; 
const uint16_t CHANGE_LENGTH_FOURHUNDRED = PULSE_COUNT_FOURHUNDRED / CHANGE_PER_PHASE;
const uint8_t DELAY_RESOLUTION_US = 10;

InputState getInputState()
{
    uint8_t reading1 = (PIND & BUTTON_MASK);

    _delay_ms(DEBOUNCE_DELAY);

    uint8_t reading2 = (PIND & BUTTON_MASK);

    bool signalIsHigh = reading1;
    bool signalIsStable = (reading1 == reading2);

    InputState newState;

    if (signalIsHigh && signalIsStable)
    {
        newState = InputState::PRESSED;
    }
    else
    {
        newState = InputState::RELEASED;
    }
    return newState;
}

void goForward()
{
    PORTA &= ~BACKWARD;
    PORTA |= FORWARD;
}

void stopForward()
{
    PORTA &= ~FORWARD;
}

void goBackward()
{
    PORTA &= ~FORWARD;
    PORTA |= BACKWARD;
}

void stopBackward()
{
    PORTA &= ~BACKWARD;
}

void goForwardSecondMotor()
{
    PORTA |= FORWARD_SECOND_MOTOR;
}

void stopForwardSecondMotor()
{
    PORTA &= ~FORWARD_SECOND_MOTOR;
}

void myDelayUs(uint16_t us)
{
    for (uint16_t i = 0; i < us / DELAY_RESOLUTION_US; i++)
    {
        _delay_us(DELAY_RESOLUTION_US);
    }
}

int main()
{
    DDRA |= FORWARD | BACKWARD | FORWARD_SECOND_MOTOR | BACKWARD_SECOND_MOTOR;

    DDRD &= ~BUTTON_MASK;

    while (true)
    {
        uint16_t high = 0;
        uint16_t low = PULSE_LENGTH_US_SIXTY;         
        for (uint16_t i = 0; i < PULSE_COUNT_SIXTY; i++) 
        {
            goForward();
            goForwardSecondMotor();
            myDelayUs(high);
            stopForward();
            stopForwardSecondMotor();
            myDelayUs(low);
            if ((i + 1) % CHANGE_LENGTH_SIXTY == 0) 
            {
                high += DUTY_ADJUST_SIXTY;
                low -= DUTY_ADJUST_SIXTY;
            }
        }

        high = 0;
        low = PULSE_LENGTH_US_FOURHUNDRED;
        for (uint16_t i = 0; i < PULSE_COUNT_FOURHUNDRED; i++)
        {
            goForward();
            goForwardSecondMotor();
            myDelayUs(high);
            stopForward();
            stopForwardSecondMotor();
            myDelayUs(low);
            if ((i + 1) % CHANGE_LENGTH_FOURHUNDRED == 0) 
            {
                high += DUTY_ADJUST_FOURHUNDRED;
                low -= DUTY_ADJUST_FOURHUNDRED;
            }
        }
    }
}
