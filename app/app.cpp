#include "menu.h"
#include "pathfinding_mode.h"
#include "corners_detector.h"

int main()
{
    Menu::initialiseMenu();

    while (true)
    {
        Menu::updateStep();
        Menu::executeStep();
    }
}
