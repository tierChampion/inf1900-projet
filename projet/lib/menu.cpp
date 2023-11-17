#include "menu.h"

#include "avr/interrupt.h"

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur

bool Menu::_isInitialised = false;
Button Menu::_modeButton = Button(GeneralInterruptType::INT_2, true);
Button Menu::_selectionButton = Button(GeneralInterruptType::INT_0, false);
Button Menu::_validationButton = Button(GeneralInterruptType::INT_1, false);
MenuStep Menu::_step;
bool Menu::_isYes;
uint8_t Menu::_line;
uint8_t Menu::_column;

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
    Menu::_selectionButton.setSenseControl(SenseControl::ANY_EDGE);
    Menu::_validationButton = Button(GeneralInterruptType::INT_1, false);
    Menu::_validationButton.setSenseControl(SenseControl::ANY_EDGE);
    Menu::_modeButton = Button(GeneralInterruptType::INT_2, true);
    Menu::_modeButton.setSenseControl(SenseControl::ANY_EDGE);

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
}

void Menu::interrupt1()
{
    Menu::_validationButton.update();
}

void Menu::interrupt2()
{
    Menu::_modeButton.update();
}

void Menu::updateStep()
{
    if (!Menu::_isInitialised)
        initialiseMenu();

    // corners
    if (Menu::_modeButton.getIsPressed())
    {
        Menu::_step = MenuStep::CORNERS;
    }

    // switch to line select
    if ((Menu::_step == MenuStep::INIT || Menu::_step == MenuStep::CORNERS) && 
    (Menu::_selectionButton.getIsPressed() && Menu::_selectionButton.getHasChanged()))
    {
        Menu::_step = MenuStep::LINE;
        Menu::_line = 0;
        Menu::_column = 0;
    }

    // line select
    else if (Menu::_step == MenuStep::LINE_RELEASE && Menu::_validationButton.getIsPressed())
    {
        Menu::_step = MenuStep::LINE;
        Menu::_line = (Menu::_line + 1) % 4;
    }
    else if (Menu::_step == MenuStep::LINE && !Menu::_validationButton.getIsPressed())
    {
        Menu::_step = MenuStep::LINE_RELEASE;
    }

    // switch to column select
    else if ((Menu::_step == MenuStep::LINE_RELEASE || Menu::_step == MenuStep::LINE) && 
    (Menu::_selectionButton.getIsPressed() && Menu::_selectionButton.getHasChanged()))
    {
        Menu::_step = MenuStep::COLUMN;
    }

    // column select
    else if (Menu::_step == MenuStep::COLUMN_RELEASE && Menu::_validationButton.getIsPressed())
    {
        Menu::_step = MenuStep::COLUMN;
        Menu::_column = (Menu::_column + 1) % 7;
    }
    else if (Menu::_step == MenuStep::COLUMN && !Menu::_validationButton.getIsPressed())
    {
        Menu::_step = MenuStep::COLUMN_RELEASE;
    }

    // switch to confirmation
    else if ((Menu::_step == MenuStep::COLUMN_RELEASE || Menu::_step == MenuStep::COLUMN) && 
    (Menu::_selectionButton.getIsPressed() && Menu::_selectionButton.getHasChanged()))
    {
        Menu::_step = MenuStep::CONFIRM;
    }

    // confirmation select
    else if (Menu::_step == MenuStep::CONFIRM_RELEASE && Menu::_selectionButton.getIsPressed())
    {
        Menu::_isYes = !Menu::_isYes;
        Menu::_step = MenuStep::CONFIRM;
    }
    else if (Menu::_step == MenuStep::CONFIRM && !Menu::_selectionButton.getIsPressed())
    {
        Menu::_step = MenuStep::CONFIRM_RELEASE;
    }

    // switch to path
    else if ((Menu::_step == MenuStep::CONFIRM_RELEASE || Menu::_step == MenuStep::CONFIRM) && 
    (Menu::_validationButton.isButtonPressed() && Menu::_validationButton.getHasChanged()))
    {
        if (_isYes) {
        Menu::_step = MenuStep::PATH;
        }
        else {
            Menu::_step = MenuStep::LINE;
            _line = 0;
            _column = 0;
        }
    }
}

void Menu::executeStep()
{

    switch (Menu::_step)
    {
    case MenuStep::INIT:
        Menu::lcd.clear();
        Menu::lcd.write("CHOISIR MODE:");
        _delay_ms(600);
        PRINT("CHOISIR MODE:");
        break;
    case MenuStep::CORNERS:
        Menu::lcd.clear();
        Menu::lcd.write("(X, Y)          Z");
        _delay_ms(600);
        PRINT("(X, Y) Z");
        break;
    case MenuStep::LINE_RELEASE:
    case MenuStep::LINE:
        Menu::lcd.clear();
        char lin[32];
        sprintf(lin, "LIGNE           %u", Menu::_line);
        Menu::lcd.write(lin);
        _delay_ms(600);
        PRINT("LIGNE");
        PRINT(Menu::_line);
        break;
    case MenuStep::COLUMN_RELEASE:
    case MenuStep::COLUMN:
        Menu::lcd.clear();
        char col[32];
        sprintf(col, "COLONNE         %u", Menu::_column);
        Menu::lcd.write(col);
        _delay_ms(600);
        PRINT("COLONNE");
        PRINT(_column);
        break;
    case MenuStep::CONFIRM_RELEASE:
    case MenuStep::CONFIRM:
        Menu::lcd.clear();
        char conf[32];
        sprintf(conf, "(%u, %u) OK?      %s", Menu::_line, Menu::_column, Menu::_isYes ? "OUI" : "NON");
        Menu::lcd.write(conf);
        _delay_ms(6000);
        PRINT("(L, C) OK?");
        PRINT(_isYes ? "OUI" : "NON");
        break;
    case MenuStep::PATH:
        Menu::lcd.clear();
        Menu::lcd.write("TRAJET EN COURS ****************");
        _delay_ms(600);
        PRINT("TRAJET EN COURS");
        break;
    }
}
