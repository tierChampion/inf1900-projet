#ifndef MAP_H
#define MAP_H

#include "commons.h"

enum class Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

class MapNode
{
public:
    MapNode() : _verticalDistances(0),
                _lateralDistances(0) {}

    MapNode(uint8_t north, uint8_t south, uint8_t east, uint8_t west);

    uint8_t getCardinalDist(Direction direction) const;
    void setCardinalDist(Direction direction, uint8_t newDist);

private:
    uint8_t _verticalDistances;
    uint8_t _lateralDistances;
};

class Map
{

public:
    Map();
    const MapNode &getNode(uint8_t position) const;
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