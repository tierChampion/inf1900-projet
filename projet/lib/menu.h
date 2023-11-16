#ifndef MENU_H
#define MENU_H

#include "button.h"

enum class MenuStep
{
    INIT,
    CORNERS,
    LINE_RELEASE,
    LINE,
    COLUMN_RELEASE,
    COLUMN,
    CONFIRM_RELEASE,
    CONFIRM,
    PATH
};

class Menu
{

public:
    static void initialiseMenu();
    static void interrupt0();
    static void interrupt1();
    static void interrupt2();

    static void update();

private:
    Menu();

private:
    static Menu _menu;

    static bool _isInitialised;

    static Button _modeButton;
    static Button _selectionButton;
    static Button _validationButton;
    static MenuStep _step;
    static bool _isYes;
    static uint8_t _line;
    static uint8_t _column;
};

#endif