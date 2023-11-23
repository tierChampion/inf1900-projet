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
    PathfindingMode mode = PathfindingMode();
    mode.run(0, 2);

    Led led = Led(Port::D, PD0, PD1);
    led.setColor(LedColor::GREEN);

    // MasterNavigation nav = MasterNavigation();
    // nav.executeMovementCode(MovementCode::FORWARD_1);

}
