#ifndef PATHFINDING_MODE
#define PATHFINDING_MODE

#include "pathfinder.h"
#include "master_navigation.h"

class PathfindingMode
{

public:
    PathfindingMode();

    void run(uint8_t line, uint8_t column);

private:
    void pathfind(uint8_t line, uint8_t column, MovementCode* moves);
    void processPath(uint8_t* path, MovementCode* moves);
    void travelPath(MovementCode* moves);
    void updatePosition(MovementCode move);
    Direction updateOrientation(MovementCode move, Direction currentDir);

private:
    Pathfinder _pathfinder;
    uint8_t _x;
    uint8_t _y;
    Direction _direction;
    MasterNavigation _navigation;
};

#endif