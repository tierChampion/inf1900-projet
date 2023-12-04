#include "menu.h"
#include "avr/interrupt.h"
#include "commons.h"

/**
 * /fichier menu.cpp
 * /auteurs Thierry Champion, Nikolai Olekhnovitch, Raissa Oumarou Petitot, Rym Touati
 * /date   05 décembre 2023
 * Créé le 7 novembre 2023
 *
 * Programme qui gère un menu interactif à l'aide de boutons sur le microcontrôleur AVR, affichent
 * des informations sur l'écran lors de la détection de coin ainsi que lors de la recherche de trajets
 * et réagissent aux différentes actions effectuée dessus. Elle contrôle aussi le robot en définissant
 * des paramètres tels que la ligne et la colonne pour la recherche de trajet, puis en executant des modes spécifiques du robot
 * tel que la recherche de chemin ou l'identifiquation du coin.
 *
 * La table des etats : (a modifier)
 * +-------------+---------+--------------+
 * | ETAT ACTUEL |    ETAT FUTURE         |
 * +-------------+---------+--------------+
 * |             |         |              |
 * +-------------+---------+--------------+
 * |             |         |              |
 * |             |         |              |
 * +-------------+---------+--------------+
 */

const uint16_t LCD_DELAY = 600;
const uint8_t DISPLAY_STRING_LENGTH = 32;
bool Menu::_isInitialised = false;
Button Menu::_modeButton = Button(GeneralInterruptType::INT_2, true);
Button Menu::_selectionButton = Button(GeneralInterruptType::INT_0, false);
Button Menu::_validationButton = Button(GeneralInterruptType::INT_1, false);
MenuStep Menu::_step = MenuStep::INIT;
bool Menu::_isYes = false;
uint8_t Menu::_line = 0;
uint8_t Menu::_column = 0;
UpdateType Menu::_updateType = UpdateType::NONE;
bool Menu::_updateScreen = true;

Robot Menu::_robot = Robot();

LCM Menu::lcd(&DDRC, &PORTC);

ISR(INT0_vect)
{
    Menu::interruptRoutine0();
}

ISR(INT1_vect)
{
    Menu::interruptRoutine1();
}

ISR(INT2_vect)
{
    Menu::interruptRoutine2();
}

void Menu::initialiseMenu()
{
    _menu = Menu();
}

Menu::Menu()
{
    Menu::_selectionButton = Button(GeneralInterruptType::INT_0, false);
    Menu::_selectionButton.setSenseControl(SenseControl::FALLING_EDGE);
    Menu::_validationButton = Button(GeneralInterruptType::INT_1, false);
    Menu::_validationButton.setSenseControl(SenseControl::FALLING_EDGE);
    Menu::_modeButton = Button(GeneralInterruptType::INT_2, true);
    Menu::_modeButton.setSenseControl(SenseControl::RISING_EDGE);

    Menu::_step = MenuStep::INIT;
    Menu::_isYes = false;
    Menu::_line = 0;
    Menu::_column = 0;

    Menu::_modeButton.enable();
    Menu::_selectionButton.enable();
    Menu::_validationButton.enable();

    EventTimer::setToggling(false, LedColor::RED);

    Menu::_isInitialised = true;
}

void Menu::interruptRoutine0()
{
    Menu::_selectionButton.update();
    if (Menu::_selectionButton.getIsPressed())
    {
        Menu::_updateType = UpdateType::SELECT;
    }
}

void Menu::interruptRoutine1()
{
    Menu::_validationButton.update();
    if (Menu::_validationButton.getIsPressed())
    {
        Menu::_updateType = UpdateType::VALIDATE;
    }
}

void Menu::interruptRoutine2()
{
    Menu::_modeButton.update();
    if (Menu::_modeButton.getIsPressed())
    {
        Menu::_updateType = UpdateType::MODE;
    }
}

void Menu::updateStep()
{
    if (!Menu::_isInitialised)
    {
        PRINT("WARNING: MENU IS NOT INITIALISED!");
        return;
    }

    if (Menu::_updateType == UpdateType::NONE && Menu::_step != MenuStep::PATH)
        return;

    // corners
    if (Menu::_updateType == UpdateType::MODE)
    {
        Menu::_step = MenuStep::CORNERS;
        Menu::_updateScreen = true;
    }

    // switch to line select
    if ((Menu::_step == MenuStep::INIT || Menu::_step == MenuStep::CORNERS) &&
        (Menu::_updateType == UpdateType::SELECT))
    {
        Menu::_step = MenuStep::LINE;
        Menu::_line = 0;
        Menu::_column = 0;
        Menu::_updateScreen = true;
        EventTimer::setToggling(false);
    }

    // line select
    else if (Menu::_step == MenuStep::LINE && Menu::_updateType == UpdateType::SELECT)
    {
        Menu::_line = (Menu::_line + 1) % MAP_HEIGHT;
        Menu::_updateScreen = true;
    }

    // switch to column select
    else if ((Menu::_step == MenuStep::LINE) &&
             (Menu::_updateType == UpdateType::VALIDATE))
    {
        Menu::_step = MenuStep::COLUMN;
        Menu::_updateScreen = true;
    }

    // column select
    else if (Menu::_step == MenuStep::COLUMN && Menu::_updateType == UpdateType::SELECT)
    {
        Menu::_column = (Menu::_column + 1) % MAP_WIDTH;
        Menu::_updateScreen = true;
    }

    // switch to confirmation
    else if ((Menu::_step == MenuStep::COLUMN) &&
             (Menu::_updateType == UpdateType::VALIDATE))
    {
        Menu::_step = MenuStep::CONFIRM;
        Menu::_updateScreen = true;
    }

    // confirmation select
    else if (Menu::_step == MenuStep::CONFIRM && Menu::_updateType == UpdateType::SELECT)
    {
        Menu::_isYes = !Menu::_isYes;
        Menu::_updateScreen = true;
    }

    // switch to path
    else if ((Menu::_step == MenuStep::CONFIRM) &&
             (Menu::_updateType == UpdateType::VALIDATE))
    {
        if (_isYes)
        {
            Menu::_step = MenuStep::PATH;
        }
        else
        {
            Menu::_step = MenuStep::LINE;
            _line = 0;
            _column = 0;
        }
        Menu::_updateScreen = true;
    }

    else if ((Menu::_step == MenuStep::PATH))
    {
        Menu::_step = MenuStep::LINE;
        Menu::_updateScreen = true;
    }

    _updateType = UpdateType::NONE;
}

void Menu::executeStep()
{
    if (!Menu::_updateScreen)
        return;

    switch (Menu::_step)
    {
    case MenuStep::INIT:
        Menu::lcd.clear();
        Menu::lcd.write("CHOISIR MODE:");
        _delay_ms(LCD_DELAY);
        PRINT("CHOISIR MODE:");
        break;
    case MenuStep::CORNERS:
        EventTimer::setToggling(false);
        Menu::lcd.clear();
        Menu::lcd.write("...");
        _delay_ms(LCD_DELAY);
        PRINT("DETECTION  EN COURS");
        Menu::_modeButton.disable();
        Menu::_selectionButton.disable();
        Menu::_validationButton.disable();
        Menu::lcd.write(Menu::_robot.runCornerMode());
        _delay_ms(LCD_DELAY);
        Menu::_modeButton.enable();
        Menu::_selectionButton.enable();
        Menu::_validationButton.enable();
        break;
    case MenuStep::LINE:
        Menu::lcd.clear();
        char lin[DISPLAY_STRING_LENGTH];
        sprintf(lin, "LIGNE           %u", Menu::_line + 1);
        Menu::lcd.write(lin);
        _delay_ms(LCD_DELAY);
        PRINT("LIGNE");
        PRINT(Menu::_line);
        break;
    case MenuStep::COLUMN:
        Menu::lcd.clear();
        char col[DISPLAY_STRING_LENGTH];
        sprintf(col, "COLONNE         %u", Menu::_column + 1);
        Menu::lcd.write(col);
        _delay_ms(LCD_DELAY);
        PRINT("COLONNE");
        PRINT(_column);
        break;
    case MenuStep::CONFIRM:
        Menu::lcd.clear();
        char conf[DISPLAY_STRING_LENGTH];
        sprintf(conf, "(%u, %u) OK?      %s", Menu::_line + 1, Menu::_column + 1, Menu::_isYes ? "OUI" : "NON");
        Menu::lcd.write(conf);
        _delay_ms(LCD_DELAY);
        PRINT("(L, C) OK?");
        PRINT(_isYes ? "OUI" : "NON");
        break;
    case MenuStep::PATH:
        Menu::lcd.clear();
        Menu::lcd.write("**   TRAJET   ****  EN COURS  **");
        _delay_ms(LCD_DELAY);
        PRINT("TRAJET EN COURS");
        Menu::_modeButton.disable();
        Menu::_selectionButton.disable();
        Menu::_validationButton.disable();
        Menu::_robot.runPathfindingMode(Menu::_line, Menu::_column);
        Menu::_modeButton.enable();
        Menu::_selectionButton.enable();
        Menu::_validationButton.enable();
        break;
    }

    Menu::_updateScreen = false;
}
