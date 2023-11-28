#include "map.h"
#include "pathfinding_mode.h"
#include "led.h"

void pillarManipulationTest(Map &map)
{
    PRINT("INITIAL MAP:");
    map.printMap();
    PRINT("PLACED PILLAR:");
    map.placePillar(14 + 4);
    map.printMap();
    PRINT("REMOVED PILLAR:");
    map.removePillar();
    map.printMap();
}

int main()
{
    // PathfindingMode mode = PathfindingMode();
    // mode.run(1, 0);
    // mode.run(0, 3);

    // Led led = Led(Port::A, PA6, PA7);
    // led.setColor(LedColor::GREEN);

    Piezo piez = Piezo();

    while (true)
    {
        piez.successMelody();
    }
    // MasterNavigation nav = MasterNavigation();
    // nav.executeMovementCode(MovementCode::FORWARD_1);
}
