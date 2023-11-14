#ifndef MAP_H
#define MAP_H

#include "commons.h"


struct MapNode {
    uint8_t _westDistance;
    uint8_t _eastDistance;
    uint8_t _northDistance;
    uint8_t _southDistance;

    MapNode() : _westDistance(0), 
    _eastDistance(0), 
    _northDistance(0), 
    _southDistance(0) {}
};

class Map {

static const uint8_t MAP_WIDTH = 7;
static const uint8_t MAP_HEIGHT = 7;
static const uint8_t NODE_COUNT = 28;

public:
    Map();
    void placePillar(uint8_t position);
    void removePillar();

    void printMap() const;

    private:
    void initialiseMap();

private:
    MapNode _nodes[Map::NODE_COUNT];
    uint8_t _pillar;
};

#endif