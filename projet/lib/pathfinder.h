#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"

class Pathfinder
{

public:
    Pathfinder();

    bool findPath(uint8_t start, uint8_t dest, uint8_t* path);

private:
    uint8_t popBestNode();
    void handleNeighbor(uint8_t position, uint8_t neighborPos, uint8_t weight);

    void printInvertedPath(uint8_t dest) const;
    void getPath(uint8_t dest, uint8_t* path) const;

public:
    static const uint8_t MAX_PATH_LENGTH = 15; 
    // attention, s'assurer que c'est assez,
    // le max trouver est de 11 ((0, 3) vers (4, 0))
private:
    Map _map;

    uint8_t _workArray[Map::NODE_COUNT];
    uint8_t _workSize;
};

#endif