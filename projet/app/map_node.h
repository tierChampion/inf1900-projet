#ifndef MAP_NODE_H
#define MAP_NODE_H

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
    void setCardinalDist(Direction direction, uint8_t newDistance);
    void setDistance(uint8_t newDistance);
    uint8_t getDistance();
    Visited getVisited() const;
    uint8_t getPreviousNode() const;
    void setVisited(Visited visited);
    void setPreviousNode(uint8_t previousPosition);

private:
    uint8_t _verticalDistances;
    uint8_t _lateralDistances;
    uint8_t _distance;
    uint8_t _travelSettings;
};
#endif