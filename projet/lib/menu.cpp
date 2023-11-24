#include "menu.h"

#include "avr/interrupt.h"

#define DEMO_DDR DDRC   // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT PORTC // Port AVR occup� par l'afficheur

const uint16_t LCD_DELAY = 600;

bool Menu::_isInitialised = false;
Button Menu::_modeButton = Button(GeneralInterruptType::INT_2, true);
Button Menu::_selectionButton = Button(GeneralInterruptType::INT_0, false);
Button Menu::_validationButton = Button(GeneralInterruptType::INT_1, false);
MenuStep Menu::_step = MenuStep::INIT;
bool Menu::_isYes = false;
uint8_t Menu::_line = 0;
uint8_t Menu::_column = 0;
UpdateType Menu::_updateType = UpdateType::NONE;
bool Menu::_update = true;

LCM Menu::lcd(&DEMO_DDR, &DEMO_PORT);

ISR(INT0_vect)
{
    Menu::interrupt0();
}

ISR(INT1_vect)
{
    Menu::interrupt1();
}

ISR(INT2_vect)
{
    Menu::interrupt2();
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

    Menu::_isInitialised = true;
}

void Menu::interrupt0()
{
    Menu::_selectionButton.update();
    if (Menu::_selectionButton.getIsPressed())
    {
        Menu::_updateType = UpdateType::SELECT;
    }
}

void Menu::interrupt1()
{
    Menu::_validationButton.update();

    if (Menu::_validationButton.getIsPressed())
    {
        Menu::_updateType = UpdateType::VALIDATE;
    }
}

void Menu::interrupt2()
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
        initialiseMenu();

    if (Menu::_updateType == UpdateType::NONE)
        return;

    // corners
    if (Menu::_updateType == UpdateType::MODE)
    {
        Menu::_step = MenuStep::CORNERS;
        Menu::_update = true;
    }

    // switch to line select
    if ((Menu::_step == MenuStep::INIT || Menu::_step == MenuStep::CORNERS) &&
        (Menu::_updateType == UpdateType::SELECT))
    {
        Menu::_step = MenuStep::LINE;
        Menu::_line = 0;
        Menu::_column = 0;
        Menu::_update = true;
    }

    // line select
    else if (Menu::_step == MenuStep::LINE && Menu::_updateType == UpdateType::VALIDATE)
    {
        Menu::_line = (Menu::_line + 1) % 4;
        Menu::_update = true;
    }

    // switch to column select
    else if ((Menu::_step == MenuStep::LINE) &&
             (Menu::_updateType == UpdateType::SELECT))
    {
        Menu::_step = MenuStep::COLUMN;
        Menu::_update = true;
    }

    // column select
    else if (Menu::_step == MenuStep::COLUMN && Menu::_updateType == UpdateType::VALIDATE)
    {
        Menu::_column = (Menu::_column + 1) % 7;
        Menu::_update = true;
    }

    // switch to confirmation
    else if ((Menu::_step == MenuStep::COLUMN) &&
             (Menu::_updateType == UpdateType::SELECT))
    {
        Menu::_step = MenuStep::CONFIRM;
        Menu::_update = true;
    }

    // confirmation select
    else if (Menu::_step == MenuStep::CONFIRM && Menu::_updateType == UpdateType::SELECT)
    {
        Menu::_isYes = !Menu::_isYes;
        Menu::_update = true;
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
        Menu::_update = true;
    }

    _updateType = UpdateType::NONE;
}

void Menu::executeStep()
{

    if (!Menu::_update)
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
        Menu::lcd.clear();
        Menu::lcd.write("(X, Y)          Z");
        _delay_ms(LCD_DELAY);
        PRINT("(X, Y) Z");
        break;
    case MenuStep::LINE_RELEASE:
    case MenuStep::LINE:
        Menu::lcd.clear();
        char lin[32];
        sprintf(lin, "LIGNE           %u", Menu::_line + 1);
        Menu::lcd.write(lin);
        _delay_ms(LCD_DELAY);
        PRINT("LIGNE");
        PRINT(Menu::_line);
        break;
    case MenuStep::COLUMN_RELEASE:
    case MenuStep::COLUMN:
        Menu::lcd.clear();
        char col[32];
        sprintf(col, "COLONNE         %u", Menu::_column + 1);
        Menu::lcd.write(col);
        _delay_ms(LCD_DELAY);
        PRINT("COLONNE");
        PRINT(_column);
        break;
    case MenuStep::CONFIRM_RELEASE:
    case MenuStep::CONFIRM:
        Menu::lcd.clear();
        char conf[32];
        sprintf(conf, "(%u, %u) OK?      %s", Menu::_line + 1, Menu::_column + 1, Menu::_isYes ? "OUI" : "NON");
        Menu::lcd.write(conf);
        _delay_ms(LCD_DELAY);
        PRINT("(L, C) OK?");
        PRINT(_isYes ? "OUI" : "NON");
        break;
    case MenuStep::PATH:
        Menu::lcd.clear();
        Menu::lcd.write("TRAJET EN COURS ****************");
        _delay_ms(LCD_DELAY);
        PRINT("TRAJET EN COURS");
        break;
    }

    Menu::_update = false;
}
