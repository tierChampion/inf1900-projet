#ifndef MENU_H
#define MENU_H

#include "button.h"
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

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

enum class UpdateType {
    NONE,
    MODE,
    SELECT,
    VALIDATE
};

class Menu
{

public:
    static void initialiseMenu();
    static void interrupt0();
    static void interrupt1();
    static void interrupt2();

    static void updateStep();
    static void executeStep();

private:
    Menu();

private:
    static Menu _menu;

    static bool _isInitialised;

    static Button _modeButton;
    static Button _selectionButton;
    static Button _validationButton;
    static MenuStep _step;
    static UpdateType _updateType;
    static bool _updateScreen;
    static uint8_t _line;
    static uint8_t _column;
    static bool _isYes;
    static LCM lcd;
};

#endif