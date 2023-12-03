#include "menu.h"
#include "pathfinding_mode.h"
#include "corners_detector.h"

int main()
{
    // MasterNavigation nav;

    // // _delay_ms(2000);
    // // nav.driveToIntersection(false); // centering calib

    // _delay_ms(2000);
    // nav.driveOneUnit(); // one unit, calibrated or not

    // Map map;
    // map.printMap();

    Menu::initialiseMenu();

    while (true)
    {
        Menu::updateStep();
        Menu::executeStep();
    }
}
