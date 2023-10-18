///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb1.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
///
/// Description du programme: Le but du programme est d'allumer une DEL bicolore en vert pour la durée de deux
///                            secondes une fois que le bouton est appuyé pour la 3ème fois.
///
/// La borne négative de la DEL est reliée à la broche A0 et la borne positive de la DEL est reliée à la broche A1.
/// A1----> borne +
/// A0----> borne -
///
/// La broche D2 est en entree et est relie au boutton poussoir.
/// La broche A0 et A1 est en sortie.
/*La table des etats :
+----------------+--------+----------------+----------+
|  ETAT ACTUEL   |   T    |  ETAT FUTURE   |Sortie(A0)|  T: Si l'interrupteur est appuyé (1:oui et 0:non)
+----------------+--------+----------------+----------+ Sortie: la DEL allumee en vert ou eteinte.
| NOT_PRESSED    |      0 | NOT_PRESSED    |     0    |
| NOT_PRESSED    |      1 | PRESSED_ONCE   |     0    |
| PRESSED_ONCE   |      0 | PRESSED_ONCE   |     0    |
| PRESSED_ONCE   |      1 | PRESSED_TWICE  |     0    |
| PRESSED_TWICE  |      0 | PRESSED_TWICE  |     0    |
| PRESSED_TWICE  |      1 | PRESSED_THRICE |     0    |
| PRESSED_THRICE |      x | NOT_PRESSED    |     1    |
+----------------+--------+----------------+----------+
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

const uint8_t MASK_A0 = (1 << PA0);
const uint8_t MASK_A1 = (1 << PA1);
const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 10;
const uint16_t TIME_LIGHTING = 2000;

enum class ButtonState
{
    NOT_PRESSED,
    PRESSED_ONCE,
    PRESSED_TWICE,
    PRESSED_THRICE
};

ButtonState etat = ButtonState::NOT_PRESSED;
bool wasButtonPressed = false;

bool isButtonPressed()
{
    uint8_t lecture1 = PIND & (1 << PD2);
    _delay_ms(DEBOUNCE_DELAY);
    uint8_t lecture2 = PIND & (1 << PD2);
    return (lecture1 && lecture2);
}

void turnOnLed()
{
    PORTA &= ~MASK_A1;
    PORTA |= MASK_A0;
}

void turnOffLed()
{
    PORTA &= ~(MASK_A0 | MASK_A1);
}

void updateState()
{
    bool buttonPressed = isButtonPressed();
    bool isNewButtonClick = buttonPressed && !wasButtonPressed;

    if (isNewButtonClick)
    {
        wasButtonPressed = true;
        switch (etat)
        {
        case ButtonState::NOT_PRESSED:
            etat = ButtonState::PRESSED_ONCE;
            break;
        case ButtonState::PRESSED_ONCE:
            etat = ButtonState::PRESSED_TWICE;
            break;
        case ButtonState::PRESSED_TWICE:
            etat = ButtonState::PRESSED_THRICE;
            break;
        default:
            break;
        }
    }
    else if (!buttonPressed)
    {
        wasButtonPressed = false;
        switch (etat)
        {
        case ButtonState::PRESSED_THRICE:
            etat = ButtonState::NOT_PRESSED;
        default:
            break;
        }
    }
}

void executeState()
{
    switch (etat)
    {
    case ButtonState::PRESSED_THRICE:
        turnOnLed();
        _delay_ms(TIME_LIGHTING);
        turnOffLed();
    default:
        break;
    }
}

int main()
{
    DDRA |= MASK_A0 | MASK_A1;
    DDRD &= ~MASK_INTERRUPT;

    while (true)
    {
        updateState();
        executeState();
    }
}
