#include "menu.h"

#include "avr/interrupt.h"

bool Menu::_isInitialised = false;
Button Menu::_modeButton = Button(GeneralInterruptType::INT_2, true);
Button Menu::_selectionButton = Button(GeneralInterruptType::INT_0, false);
Button Menu::_validationButton = Button(GeneralInterruptType::INT_1, false);
MenuStep Menu::_step;
bool Menu::_isYes;
uint8_t Menu::_line;
uint8_t Menu::_column;

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
        PRINT("CHOISIR MODE:");
        break;
    case MenuStep::CORNERS:
        PRINT("(X, Y) Z");
        break;
    case MenuStep::LINE_RELEASE:
    case MenuStep::LINE:
        PRINT("LIGNE");
        PRINT(Menu::_line);
        break;
    case MenuStep::COLUMN_RELEASE:
    case MenuStep::COLUMN:
        PRINT("COLONNE");
        PRINT(_column);
        break;
    case MenuStep::CONFIRM_RELEASE:
    case MenuStep::CONFIRM:
        PRINT("(L, C) OK?");
        PRINT(_isYes ? "OUI" : "NON");
        break;
    case MenuStep::PATH:
        PRINT("TRAJET EN COURS...");
        break;
    }
}
