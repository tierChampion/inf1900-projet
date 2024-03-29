#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"

class Pathfinder
{

public:
    Pathfinder();

    void findPath(uint8_t start, uint8_t dest, uint8_t *path);
    void modifyMap(uint8_t pipePos);
    void resetMap();

    Map getMap() { return _map; }

private:
    uint8_t popBestNode();
    void handleNeighbor(uint8_t position, uint8_t neighborPos, uint8_t weight);

    void printInvertedPath(uint8_t dest) const;
    void getPath(uint8_t dest, uint8_t *path) const;

public:
    static const uint8_t MAX_PATH_LENGTH = 16;

private:
    Map _map;

    uint8_t _workArray[NODE_COUNT];
    uint8_t _workSize;
};

#endif
