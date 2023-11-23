#include "map.h"
#include "pathfinding_mode.h"

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

    // mode.run(3, 0);

    DistanceSensor sensor = DistanceSensor();

    while (true)
    {
        PRINT(sensor.isClose() ? "OUI" : "NON");
    }
}
