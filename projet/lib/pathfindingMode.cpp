#include "pathfindingMode.h"

PathfindingMode::PathfindingMode() : _pathfinder(Pathfinder()),
                                     _x(0), _y(0), 
                                     _orientation(Direction::SOUTH) {}

void PathfindingMode::run(uint8_t line, uint8_t column)
{
    pathfind(line, column);
    travelPath();
}

void PathfindingMode::pathfind(uint8_t line, uint8_t column)
{
    uint8_t start = _y * Map::MAP_WIDTH + _x;
    uint8_t dest = line * Map::MAP_WIDTH + column;

    _pathfinder.findPath(start, dest);

    travelPath();
}

void PathfindingMode::travelPath()
{
    // todo
}
