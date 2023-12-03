#ifndef MAP_H
#define MAP_H

#include "map_node.h"

class Map
{

public:
    Map();
    void reset();
    const MapNode &operator[](uint8_t position) const;
    MapNode &operator[](uint8_t position);

    void placePillar(uint8_t position);
    void removePillar();

    static uint8_t getPositionX(uint8_t position);
    static uint8_t getPositionY(uint8_t position);

    static uint8_t getNorthPosition(uint8_t position);
    static uint8_t getSouthPosition(uint8_t position);
    static uint8_t getEastPosition(uint8_t position);
    static uint8_t getWestPosition(uint8_t position);

    bool isLinePosition(uint8_t position);

    void printMap() const;

private:
    void initialiseMap();

private:
    MapNode _nodes[NODE_COUNT];
    uint8_t _pillar;
};

#endif