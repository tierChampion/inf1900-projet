#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"

enum class Visited : uint8_t {
    UNKNOWN = 0,
    VISITED = 1,
    FINISHED = 2
};

struct WorkNode
{
    uint8_t _distance;
    uint8_t _travelSettings;

    WorkNode();

    Visited getVisited() const;
    uint8_t getPrev() const;
    void setVisited(Visited visited);
    void setPrev(uint8_t prevPos);
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