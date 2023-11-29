#include "menu.h"
#include "pathfinding_mode.h"
#include "corners_detector.h"

int main()
{
    MasterNavigation nav = MasterNavigation();
    Piezo piezo = Piezo();

    // MAIN CALIBRATION AUTO:

    nav.driveToIntersection(true);

    _delay_ms(350);

    nav.driveToIntersection();

    // MAIN FINAL:

    //PathfindingMode mode1 = PathfindingMode(&nav, &piezo);
    //CornersDetector mode2 = CornersDetector(&nav, &piezo);

    //mode1.setPosition(3, 6, Direction::EAST);

    //Menu::initialiseMenu(&mode1, &mode2);

    // while (true)
    // {
    //     Menu::updateStep();
    //     Menu::executeStep();
    // }
}
