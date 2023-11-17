#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"

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
    Map _map;
    uint8_t _workArray[Map::NODE_COUNT];
    uint8_t _workSize;
};

#endif