#ifndef PATHFINDING_MODE
#define PATHFINDING_MODE

#include "pathfinder.h"
#include "master_navigation.h"
#include "piezo.h"

class PathfindingMode
{

public:
    PathfindingMode();

    void run(uint8_t line, uint8_t column, MasterNavigation *navigation, Piezo* piezo);

    void setPosition(uint8_t line, uint8_t column, Direction dir);
    uint8_t getPosition() { return _position; }

private:
    void pathfind(uint8_t line, uint8_t column, MovementCode *moves);

    void processPath(uint8_t *path, MovementCode *moves);
    bool travelPath(MovementCode *moves, MasterNavigation* navigation, Piezo* piezo);

    void foundPillar(MovementCode currentMove, Piezo* piezo);
    void finishedPath(Piezo* piezo);

    uint8_t updatePosition(MovementCode move, Direction currentDir, uint8_t currentPos);
    Direction updateOrientation(MovementCode move, Direction currentDir);

private:
    DistanceSensor _distSensor;
    Pathfinder _pathfinder;
    uint8_t _position;
    Direction _direction;
};

#endif