/////////////////
// Auteurs: Thierry Champion et Nikolai Olekhnovitch
//
// Description: Implementation d'une machine a etats logicielle. Tout d'abord, la DEL est rouge et
//      et suite a un appuie devient ambre. Lors du relachement, elle devient verte. Un autre appuie
//      la rend rouge. Le prochain relachement eteint la DEL. Un dernier appuie l'allume verte et
//      le dernier relachement revoit au dernier etat, donc la DEL s'allume a rouge.
//
// Entrees/Sorties: La broche D2 est en entree et est utilisee pour la lecture du bouton-poussoir.
//      Les broches A0 et A1 sont en sortie et sont utilisees pour le controle de la DEL.
//      La broche A0 est connectee au port positif de la DEL et la broche A1 est connectee
//      au port negatif de la DEL.
//
// ╔══════════╦═══════=╦══════════╦═════════════╗
// ║  STATE   ║ PIN D2 ║   NEXT   ║     LED     ║
// ╠══════════╬═══════=╬══════════╬═════════════╣
// ║ INIT     ║      0 ║ INIT     ║ 10 (RED)    ║
// ║ INIT     ║      1 ║ PRESS1   ║ 10 (RED)    ║
// ║ PRESS1   ║      0 ║ RELEASE1 ║ 11 (YELLOW) ║
// ║ PRESS1   ║      1 ║ PRESS1   ║ 11 (YELLOW) ║
// ║ RELEASE1 ║      0 ║ RELEASE1 ║ 01 (GREEN)  ║
// ║ RELEASE1 ║      1 ║ PRESS2   ║ 01 (GREEN)  ║
// ║ PRESS2   ║      0 ║ RELEASE2 ║ 10 (RED)    ║
// ║ PRESS2   ║      1 ║ PRESS2   ║ 10 (RED)    ║
// ║ RELEASE2 ║      0 ║ RELEASE2 ║ 00 (OFF)    ║
// ║ RELEASE2 ║      1 ║ PRESS3   ║ 00 (OFF)    ║
// ║ PRESS3   ║      0 ║ RELEASE3 ║ 01 (GREEN)  ║
// ║ PRESS3   ║      1 ║ INIT     ║ 01 (GREEN)  ║
// ╚══════════╩═════=══╩══════════╩═════════════╝
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
};

const uint8_t BUTTON_MASK = (1 << PD2);
const uint8_t LED_RED = (1 << PA0);
const uint8_t LED_GREEN = (1 << PA1);
const uint8_t LED_OFF = (1 << PA0) | (1 << PA1);
const uint8_t DEBOUNCE_DELAY = 10;
const uint8_t RED_DELAY = 2;
const uint8_t GREEN_DELAY = 8;
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

void openRedLed()
{
    PORTA &= ~LED_GREEN;
    PORTA |= LED_RED;
}

void closeLed()
{
    PORTA &= ~LED_OFF;
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
                state = FsmState::INIT;
            }
            break;
    }
}

void executeFsm()
{
    switch (state)
    {
    case FsmState::INIT:
    {
        openRedLed();
    }
    break;

    case FsmState::PRESS1:
    {
        openGreenLed();
        _delay_ms(GREEN_DELAY);
        openRedLed();
        _delay_ms(RED_DELAY);
    }
    break;

    case FsmState::RELEASE1:
    {
        openGreenLed();
    }
    break;

    case FsmState::PRESS2:
    {
        openRedLed();
    }
    break;

    case FsmState::RELEASE2:
    {
        closeLed();
    }
    break;

    case FsmState::PRESS3:
    {
        openGreenLed();
    }
    break;
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
