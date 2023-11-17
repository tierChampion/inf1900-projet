#ifndef PATHFINDING_MODE
#define PATHFINDING_MODE

#include "pathfinder.h"

class PathfindingMode
{

public:
    PathfindingMode();

    void run(uint8_t line, uint8_t column);

private:
    void pathfind(uint8_t line, uint8_t column);
    void travelPath();

private:
    uint8_t _x;
    uint8_t _y;
    Direction _orientation;
    Pathfinder _pathfinder;
};

#endif