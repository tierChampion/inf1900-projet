#include "menu.h"
#include "pathfinding_mode.h"
#include "corners_detector.h"

int main()
{
    // MAIN FINAL:

    Menu::initialiseMenu();

    while (true)
    {
        Menu::updateStep();
        Menu::executeStep();
    }
    // Pathfinder finder;

    //     finder.modifyMap(16);
    //     uint8_t path[15];
    //     //finder.findPath(0, 2, path);

    //     finder.resetMap();
    //     //finder.findPath(2, 0, path);
}
