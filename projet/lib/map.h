#ifndef MAP_H
#define MAP_H

#include "commons.h"

struct MapNode
{
    uint8_t _westDistance;
    uint8_t _eastDistance;
    uint8_t _northDistance;
    uint8_t _southDistance;

    MapNode() : _westDistance(0),
                _eastDistance(0),
                _northDistance(0),
                _southDistance(0) {}
};

class Map
{

public:
    Map();
    const MapNode& getNode(uint8_t position) const;
    void placePillar(uint8_t position);
    void removePillar();

    void printMap() const;

private:
    void initialiseMap();

public:
    static const uint8_t MAP_WIDTH = 7;
    static const uint8_t MAP_HEIGHT = 4;
    static const uint8_t NODE_COUNT = 28;

private:
    MapNode _nodes[Map::NODE_COUNT];
    uint8_t _pillar;
};

uint8_t getPositionX(uint8_t position);
uint8_t getPositionY(uint8_t position);

uint8_t getNorthId(uint8_t position);
uint8_t getSouthId(uint8_t position);
uint8_t getEastId(uint8_t position);
uint8_t getWestId(uint8_t position);

#endif