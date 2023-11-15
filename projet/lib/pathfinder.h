#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"

enum class VisitedState : uint8_t {
    UNKNOWN = 0,
    VISITED = 1,
    FINISHED = 2
};

struct WorkNode
{
    uint8_t _prev;
    uint8_t _distance;
    VisitedState _visited;

    WorkNode() : _distance(0xFF),
                 _prev(0xFF),
                 _visited(VisitedState::UNKNOWN) {}
};

class Pathfinder
{

public:
    Pathfinder();
    void findPath(const Map& map, uint8_t start, uint8_t dest);

private:
    uint8_t popBestNode();
    void handleNeighbor(uint8_t position, uint8_t neighborPos, uint8_t weight);

    void printInvertedPath(uint8_t dest) const;

private:
    WorkNode _workMap[Map::NODE_COUNT];
    uint8_t _workArray[Map::NODE_COUNT];
    uint8_t _workSize;
};

#endif