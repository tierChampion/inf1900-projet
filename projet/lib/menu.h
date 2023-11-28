#ifndef MENU_H
#define MENU_H

#include "button.h"
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

#include "pathfinding_mode.h"
#include "corners_detector.h"

enum class MenuStep
{
    INIT,
    CORNERS,
    LINE,
    COLUMN,
    CONFIRM,
    PATH
};

enum class UpdateType
{
    NONE,
    MODE,
    SELECT,
    VALIDATE
};

class Menu
{

public:
    static void initialiseMenu(PathfindingMode *pathMode,
                               CornersDetector *cornerMode);
    static void interrupt0();
    static void interrupt1();
    static void interrupt2();

    static void updateStep();
    static void executeStep();

private:
    Menu(PathfindingMode *pathMode,
         CornersDetector *cornerMode);

private:
    static Menu _menu;

    static bool _isInitialised;

    static PathfindingMode *_pathMode;
    static CornersDetector *_cornerMode;

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