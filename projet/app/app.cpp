#include "menu.h"
#include "pathfinding_mode.h"
#include "corners_detector.h"

int main()
{
    // MAIN CALIBRATION AUTO:

    // MasterNavigation nav = MasterNavigation();
    // Piezo piezo = Piezo();

    // nav.driveToIntersection(true);

    // _delay_ms(350);

    // nav.driveOneUnit();

    // MAIN FINAL:

    Menu::initialiseMenu();

    while (true)
    {
        Menu::updateStep();
        Menu::executeStep();
    }
}
