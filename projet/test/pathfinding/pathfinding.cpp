#include "map.h"
#include "pathfinder.h"

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
    Map map = Map();
    Pathfinder finder = Pathfinder();

    map.placePillar(16);

    finder.findPath(map, 14, 22);
}
