/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: Allumer la DEL libre en vert pendant 2 secondes suite a trois appuis du bouton-poussoir.
//
// Entrees/Sorties: La broche D2 est en entree et est utilisee pour la lecture du bouton-poussoir.
//      Les broches A0 et A1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche A0 est connectee au port positif de la DEL et la broche A1 est connectee
//      au port negatif de la DEL.
//
// ╔══════════╦════════╦══════════╦═════╗
// ║  STATE   ║ Pin D2 ║   NEXT   ║ LED ║
// ╠══════════╬════════╬══════════╬═════╣
// ║ INIT     ║     0  ║ INIT     ║   0 ║
// ║ INIT     ║     1  ║ PRESS1   ║   0 ║
// ║ PRESS1   ║     0  ║ RELEASE1 ║   0 ║
// ║ PRESS1   ║     1  ║ PRESS1   ║   0 ║
// ║ RELEASE1 ║     0  ║ RELEASE1 ║   0 ║
// ║ RELEASE1 ║     1  ║ PRESS2   ║   0 ║
// ║ PRESS2   ║     0  ║ RELEASE2 ║   0 ║
// ║ PRESS2   ║     1  ║ PRESS2   ║   0 ║
// ║ RELEASE2 ║     0  ║ RELEASE2 ║   0 ║
// ║ RELEASE2 ║     1  ║ PRESS3   ║   0 ║
// ║ PRESS3   ║     0  ║ RELEASE3 ║   0 ║
// ║ PRESS3   ║     1  ║ PRESS3   ║   0 ║
// ║ RELEASE3 ║     0  ║ INIT     ║   1 ║
// ║ RELEASE3 ║     1  ║ INIT     ║   1 ║
// ╚══════════╩════════╩══════════╩═════╝
/////////////////

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

enum class InputState
{
    PRESSED,
    RELEASED
};

enum class FsmState
{
    INIT,
    PRESS1,
    RELEASE1,
    PRESS2,
    RELEASE2,
    PRESS3,
    RELEASE3
};

const uint8_t BUTTON_MASK = (1 << PD2);
const uint8_t LED_RED = (1 << PA0);
const uint8_t LED_GREEN = (1 << PA1);
const uint8_t DEBOUNCE_DELAY = 10;
const uint16_t LIGHT_DELAY = 2000;

FsmState state = FsmState::INIT;

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

void updateFsm()
{
    InputState input = getInputState();

    switch (state)
    {
        case FsmState::INIT:
            if (input == InputState::PRESSED)
            {
                state = FsmState::PRESS1;
            }
            break;

        case FsmState::PRESS1:
            if (input == InputState::RELEASED)
            {
                state = FsmState::RELEASE1;
            }
            break;

        case FsmState::RELEASE1:
            if (input == InputState::PRESSED)
            {
                state = FsmState::PRESS2;
            }
            break;

        case FsmState::PRESS2:
            if (input == InputState::RELEASED)
            {
                state = FsmState::RELEASE2;
            }
            break;

        case FsmState::RELEASE2:
            if (input == InputState::PRESSED)
            {
                state = FsmState::PRESS3;
            }
            break;

        case FsmState::PRESS3:
            if (input == InputState::RELEASED)
            {
                state = FsmState::RELEASE3;
            }
            break;

        case FsmState::RELEASE3:
            state = FsmState::INIT;
    }
}

void executeFsm()
{
    switch (state)
    {
        case FsmState::RELEASE3:
            openGreenLed();
            _delay_ms(LIGHT_DELAY);
            break;

        default:
            closeGreenLed();
    }
}

int main()
{
    DDRA |= LED_RED | LED_GREEN;

    DDRD &= ~BUTTON_MASK;

    while (true)
    {
        updateFsm();
        executeFsm();
    }
}
