#include "menu.h"
#include "pathfinding_mode.h"
#include "corners_detector.h"

int main()
{

    MasterNavigation nav = MasterNavigation();
    Piezo piezo = Piezo();

    PathfindingMode mode1 = PathfindingMode(&nav);
    CornersDetector mode2 = CornersDetector(&nav, &piezo);

    Menu::initialiseMenu(&mode1, &mode2);

    while (true)
    {
        Menu::updateStep();
        Menu::executeStep();
    }
}
