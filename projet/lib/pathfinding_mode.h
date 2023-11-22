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
    void processPath(uint8_t* path, bool isDestMiddle, MovementCode* moves);
    void travelPath(MovementCode* moves);
    uint8_t updatePosition(MovementCode move, Direction currentDir, uint8_t currentPos);
    Direction updateOrientation(MovementCode move, Direction currentDir);

private:
    Pathfinder _pathfinder;
    uint8_t _position;
    Direction _direction;
    MasterNavigation _navigation;
};

#endif