#ifndef MAP_H
#define MAP_H

#include "commons.h"

enum class Direction : uint8_t
{
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3
};

enum class Visited : uint8_t
{
    UNKNOWN = 0,
    VISITED = 1,
    FINISHED = 2
};

class MapNode
{
public:
    MapNode();
    MapNode(uint8_t north, uint8_t south, uint8_t east, uint8_t west);

    uint8_t getCardinalDist(Direction direction) const;
    void setCardinalDist(Direction direction, uint8_t newDist);
    void setDistance(uint8_t newDist);
    uint8_t getDistance();
    Visited getVisited() const;
    uint8_t getPrev() const;
    void setVisited(Visited visited);
    void setPrev(uint8_t prevPos);

private:
    uint8_t _verticalDistances;
    uint8_t _lateralDistances;
    uint8_t _distance;
    uint8_t _travelSettings;
};

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

    void printMap() const;

private:
    void initialiseMap();

public:
    static const uint8_t MAP_WIDTH = 7;
    static const uint8_t MAP_HEIGHT = 4;
    static const uint8_t NODE_COUNT = 28;
    static const uint8_t NONE = 0xFF;
    static const uint8_t DISCONNECTED = 0;

private:
    MapNode _nodes[Map::NODE_COUNT];
    uint8_t _pillar;
};

#endif