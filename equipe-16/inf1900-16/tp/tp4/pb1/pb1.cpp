///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \file   pb2.cpp
/// \authors Rym Touati et Raissa Oumarou Petitot
///
/// Description du programme: Le but de ce programme est d'initialement allumer la DEL en rouge. En appuyant sur le bouton,
///                           elle devient ambre. En relâchant, elle devient verte. En appuyant à nouveau, elle devient
///                           rouge, puis lorsqu'elle est relâchée, elle s'éteint. Une nouvelle pression sur le bouton
///                           la fait devenir verte, puis lorsqu'elle est relâchée, elle repasse au rouge, ramenant
///                           ainsi la carte mère à son état de départ pour recommencer.
///
/// La borne négative de la DEL est reliée à la broche A0 et la borne positive de la DEL est reliée à la broche A1.
/// A1----> borne +
/// A0----> borne -
/// La broche D2 est en entree et est relie au bouton poussoir
/// Les broches A0 et A1 en sortie.
///
/* La table des etats :
+-------------+---------+---------+-------------+
| ETAT ACTUEL |    ETAT FUTURE    |Sortie(A0A1) |
+-------------+---------+---------+-------------+
|             | ~Button | Button  | A0   | A1   | Button: Interrupteur de la broche D2
+-------------+---------+---------+-------------+ Sortie: La couleur affichee par la DEL
| RED_1       | RED_1   | AMBRE   | 0    | 1    |
| AMBRE       | GREEN_1 | AMBRE   | 1    | 1    |
| GREEN_1     | GREEN_1 | RED_2   | 1    | 0    |
| RED_2       | BLACK   | RED_2   | 0    | 1    |
| BLACK       | BLACK   | GREEN_2 | 0    | 0    |
| GREEN_2     | RED_1   | GREEN_2 | 1    | 0    |
+-------------+---------+---------+-------------+
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
const uint8_t MASK_INTERRUPT = (1 << PD2);
const uint8_t LED_RED = (1 << PA1);
const uint8_t LED_GREEN = (1 << PA0);
const uint8_t MASK_LED = LED_GREEN | LED_RED;
const uint8_t DEBOUNCE_DELAY = 30;
const uint8_t TIME_SWITCHING_AMBRE = 5;
enum class LEDColor
{
    RED_1,
    AMBRE,
    GREEN_1,
    RED_2,
    BLACK,
    GREEN_2
};
volatile LEDColor etat = LEDColor::RED_1;

bool readButton()
{
    return PIND & (1 << PD2);
}

// placer le bon type de signal d'interruption
// à prendre en charge en argument
ISR(INT0_vect)
{
    uint8_t lecture1 = readButton();
    // laisser un délai avant de confirmer la réponse du
    // bouton-poussoir: environ 30 ms (anti-rebond)
    _delay_ms(DEBOUNCE_DELAY);
    // se souvenir ici si le bouton est pressé ou relâché
    // changements d'état tels que ceux de la
    // semaine précédente
    uint8_t lecture2 = readButton();
    bool isButtonPushed = (lecture1 && lecture2);

    if (isButtonPushed)
        switch (etat)
        {
        case LEDColor::RED_1:
            etat = LEDColor::AMBRE;
            break;
        case LEDColor::GREEN_1:
            etat = LEDColor::RED_2;
            break;
        case LEDColor::BLACK:
            etat = LEDColor::GREEN_2;
            break;
        default:
            break;
        }
    else
        switch (etat)
        {
        case LEDColor::AMBRE:
            etat = LEDColor::GREEN_1;
            break;
        case LEDColor::RED_2:
            etat = LEDColor::BLACK;
            break;
        case LEDColor::GREEN_2:
            etat = LEDColor::RED_1;
            break;
        default:
            break;
        }
    // Voir la note plus bas pour comprendre cette instruction et son rôle
    EIFR |= (1 << INTF0);
}

void initialisation()
{
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontrôleur n'est pas prêt...
    cli();
    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA |= MASK_LED;
    DDRD &= ~MASK_INTERRUPT;
    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0);
    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00);
    // sei permet de recevoir à nouveau des interruptions.
    sei();
}

void switchLedToGreen()
{
    PORTA &= ~(LED_RED);
    PORTA |= (LED_GREEN);
}

void switchLedToRed()
{
    PORTA &= ~(LED_GREEN);
    PORTA |= (LED_RED);
}

void switchLedToAmbre()
{
    switchLedToGreen();
    _delay_ms(TIME_SWITCHING_AMBRE);
    switchLedToRed();
    _delay_ms(TIME_SWITCHING_AMBRE);
}

void executeState()
{
    switch (etat)
    {
    case LEDColor::RED_1:
    case LEDColor::RED_2:
        switchLedToRed();
        break;
    case LEDColor::AMBRE:
        switchLedToAmbre();
        break;
    case LEDColor::GREEN_1:
    case LEDColor::GREEN_2:
        switchLedToGreen();
        break;
    case LEDColor::BLACK:
        PORTA &= ~MASK_LED;
    }
}

int main()
{
    initialisation();
    while (true)
    {
        executeState();
    }
}
