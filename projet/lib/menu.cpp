#include "menu.h"

#include "avr/interrupt.h"

bool Menu::_isInitialised = false;

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
    _selectionButton = Button(GeneralInterruptType::INT_0, false);
    _selectionButton.setSenseControl(SenseControl::ANY_EDGE);
    _validationButton = Button(GeneralInterruptType::INT_1, false);
    _validationButton.setSenseControl(SenseControl::ANY_EDGE);
    _modeButton = Button(GeneralInterruptType::INT_2, true);
    _modeButton.setSenseControl(SenseControl::ANY_EDGE);

    _step = MenuStep::INIT;
    _isYes = false;
    _line = 0;
    _column = 0;

    _modeButton.enable();
    _selectionButton.enable();
    _validationButton.enable();
}

void Menu::interrupt0()
{
    _selectionButton.update();
}

void Menu::interrupt1()
{
    _validationButton.update();
}

void Menu::interrupt2()
{
    _modeButton.update();
}

void Menu::update()
{
    if (!_isInitialised)
        initialiseMenu();

    // corners
    if (_modeButton.getIsPressed())
    {
        _step = MenuStep::CORNERS;
        PRINT("COIN...");
    }

    // switch to line select
    if ((_step == MenuStep::INIT || _step == MenuStep::CORNERS) && _selectionButton.getIsPressed())
    {
        _step = MenuStep::LINE;
        _line = 0;
        _column = 0;
    }

    // line select
    if (_step == MenuStep::LINE_RELEASE && _validationButton.getIsPressed())
    {
        _step = MenuStep::LINE;
        _line = (_line + 1) % 4;
    }
    else if (_step == MenuStep::LINE && !_validationButton.getIsPressed())
    {
        _step = MenuStep::LINE_RELEASE;
    }

    // switch to column select
    if ((_step == MenuStep::LINE_RELEASE || _step == MenuStep::LINE) && _selectionButton.getIsPressed())
    {
        _step = MenuStep::COLUMN;
    }

    // column select
    if (_step == MenuStep::COLUMN_RELEASE && _validationButton.getIsPressed())
    {
        _step = MenuStep::COLUMN;
        _column = (_column + 1) % 7;
    }
    else if (_step == MenuStep::COLUMN && !_validationButton.getIsPressed())
    {
        _step = MenuStep::COLUMN_RELEASE;
    }

    // switch to confirmation
    if ((_step == MenuStep::COLUMN_RELEASE || _step == MenuStep::COLUMN) && _selectionButton.getIsPressed())
    {
        _step = MenuStep::CONFIRM;
    }

    // confirmation select
    if (_step == MenuStep::CONFIRM_RELEASE && _selectionButton.getIsPressed())
    {
        _isYes = !_isYes;
        _step = MenuStep::CONFIRM;
    } 
    else if (_step == MenuStep::CONFIRM && !_selectionButton.getIsPressed())
    {
        _step = MenuStep::CONFIRM_RELEASE;
    }

    // switch to path
    if ((_step == MenuStep::CONFIRM_RELEASE || _step == MenuStep::CONFIRM) && _validationButton.isButtonPressed())
    {
        _step = MenuStep::PATH;
    }
}
