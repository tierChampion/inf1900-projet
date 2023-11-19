#include "map.h"
#include "pathfindingMode.h"

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

    mode.run(3, 0);
}
