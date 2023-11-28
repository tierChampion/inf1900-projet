#ifndef PATHFINDING_MODE
#define PATHFINDING_MODE

#include "pathfinder.h"
#include "master_navigation.h"
#include "piezo.h"

class PathfindingMode
{

public:
    PathfindingMode(MasterNavigation* navigation);

    void run(uint8_t line, uint8_t column);

    void setPosition(uint8_t line, uint8_t column, Direction dir);

private:
    void pathfind(uint8_t line, uint8_t column, MovementCode* moves);

    void processPath(uint8_t* path, bool isDestMiddle, MovementCode* moves);
    bool travelPath(MovementCode* moves);

    void foundPillar(MovementCode currentMove);
    void finishedPath();

    uint8_t updatePosition(MovementCode move, Direction currentDir, uint8_t currentPos);
    Direction updateOrientation(MovementCode move, Direction currentDir);

private:
    MasterNavigation *_navigation;
    DistanceSensor _distSensor;
    Pathfinder _pathfinder;
    uint8_t _position;
    Direction _direction;
    Piezo _piezo;
};

#endif