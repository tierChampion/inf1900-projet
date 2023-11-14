#include "map.h"

const uint8_t NONE = 0xFF;
const uint8_t NORMAL_WEIGHT = 1;
const uint8_t YELLOW_WEIGHT = 2;
const uint8_t RED_WEIGHT = 5;

Map::Map()
{
    _pillar = NONE;

    initialiseMap();
}

const MapNode& Map::getNode(uint8_t position) const
{
    return _nodes[position];
}

void Map::placePillar(uint8_t position)
{
    _pillar = position;

    uint8_t x = getPositionX(position);
    uint8_t y = getPositionY(position);

    if (y > 0)
    {
        // north
        _nodes[getNorthId(position)]._southDistance = 0;
    }
    if (y < MAP_HEIGHT - 1)
    {
        // south
        _nodes[getSouthId(position)]._northDistance = 0;
    }
    if (x > 0)
    {
        // west
        _nodes[getWestId(position)]._eastDistance = 0;
    }
    if (x < MAP_WIDTH - 1)
    {
        // east
        _nodes[getEastId(position)]._westDistance = 0;
    }
}

void Map::removePillar()
{
    uint8_t x = getPositionX(_pillar);
    uint8_t y = getPositionY(_pillar);

    if (y > 0)
    {
        // north
        _nodes[getNorthId(_pillar)]._southDistance = _nodes[_pillar]._northDistance;
    }
    if (y < MAP_HEIGHT - 1)
    {
        // south
        _nodes[getSouthId(_pillar)]._northDistance = _nodes[_pillar]._southDistance;
    }
    if (x > 0)
    {
        // west
        _nodes[getWestId(_pillar)]._eastDistance = _nodes[_pillar]._westDistance;
    }
    if (x < MAP_WIDTH - 1)
    {
        // east
        _nodes[getEastId(_pillar)]._westDistance = _nodes[_pillar]._eastDistance;
    }

    _pillar = NONE;
}

uint8_t getPositionX(uint8_t position)
{
    return position % Map::MAP_WIDTH;
}

uint8_t getPositionY(uint8_t position)
{
    return position / Map::MAP_WIDTH;
}

uint8_t getNorthId(uint8_t position)
{
    if (getPositionY(position) == 0)
        return NONE;
    return position - Map::MAP_WIDTH;
}

uint8_t getSouthId(uint8_t position)
{
    if (getPositionY(position) == Map::MAP_HEIGHT - 1)
        return NONE;
    return position + Map::MAP_WIDTH;
}

uint8_t getEastId(uint8_t position)
{
    if (getPositionX(position) == Map::MAP_WIDTH - 1)
        return NONE;
    return position + 1;
}

uint8_t getWestId(uint8_t position)
{
    if (getPositionX(position) == 0)
        return NONE;
    return position - 1;
}

void Map::printMap() const
{
    for (uint8_t i = 0; i < NODE_COUNT; i++)
    {
        PRINT("NODE: (N, S, E, W)");
        PRINT(_nodes[i]._northDistance);
        PRINT(_nodes[i]._southDistance);
        PRINT(_nodes[i]._eastDistance);
        PRINT(_nodes[i]._westDistance);
    }
}

void Map::initialiseMap()
{
    // (1, 1)
    _nodes[0]._eastDistance = NORMAL_WEIGHT;
    _nodes[0]._southDistance = NORMAL_WEIGHT;

    // (1, 2)
    _nodes[1]._eastDistance = NORMAL_WEIGHT;
    _nodes[1]._westDistance = NORMAL_WEIGHT;

    // (1, 3)
    _nodes[2]._eastDistance = NORMAL_WEIGHT;
    _nodes[2]._westDistance = NORMAL_WEIGHT;
    _nodes[2]._southDistance = NORMAL_WEIGHT;

    // (1, 4)
    _nodes[3]._westDistance = NORMAL_WEIGHT;
    _nodes[3]._southDistance = NORMAL_WEIGHT;

    // (1, 5)
    _nodes[4]._eastDistance = NORMAL_WEIGHT;

    // (1, 6)
    _nodes[5]._eastDistance = YELLOW_WEIGHT;
    _nodes[5]._westDistance = NORMAL_WEIGHT;
    _nodes[5]._southDistance = YELLOW_WEIGHT;

    // (1, 7)
    _nodes[6]._westDistance = YELLOW_WEIGHT;
    _nodes[6]._southDistance = NORMAL_WEIGHT;

    // (2, 1)
    _nodes[7]._eastDistance = RED_WEIGHT;
    _nodes[7]._northDistance = NORMAL_WEIGHT;

    // (2, 2)
    _nodes[8]._eastDistance = NORMAL_WEIGHT;
    _nodes[8]._westDistance = RED_WEIGHT;
    _nodes[8]._southDistance = NORMAL_WEIGHT;

    // (2, 3)
    _nodes[9]._westDistance = NORMAL_WEIGHT;
    _nodes[9]._southDistance = RED_WEIGHT;
    _nodes[9]._northDistance = NORMAL_WEIGHT;

    // (2, 4)
    _nodes[10]._eastDistance = YELLOW_WEIGHT;
    _nodes[10]._northDistance = NORMAL_WEIGHT;

    // (2, 5)
    _nodes[11]._eastDistance = NORMAL_WEIGHT;
    _nodes[11]._westDistance = YELLOW_WEIGHT;
    _nodes[11]._southDistance = RED_WEIGHT;

    // (2, 6)
    _nodes[12]._eastDistance = NORMAL_WEIGHT;
    _nodes[12]._westDistance = NORMAL_WEIGHT;
    _nodes[12]._southDistance = NORMAL_WEIGHT;
    _nodes[12]._northDistance = YELLOW_WEIGHT;

    // (2, 7)
    _nodes[13]._westDistance = NORMAL_WEIGHT;
    _nodes[13]._southDistance = NORMAL_WEIGHT;
    _nodes[13]._northDistance = NORMAL_WEIGHT;

    // (3, 1)
    _nodes[14]._eastDistance = YELLOW_WEIGHT;
    _nodes[14]._southDistance = RED_WEIGHT;

    // (3, 2)
    _nodes[15]._eastDistance = NORMAL_WEIGHT;
    _nodes[15]._westDistance = YELLOW_WEIGHT;
    _nodes[15]._northDistance = NORMAL_WEIGHT;

    // (3, 3)
    _nodes[16]._eastDistance = NORMAL_WEIGHT;
    _nodes[16]._westDistance = NORMAL_WEIGHT;
    _nodes[16]._southDistance = NORMAL_WEIGHT;
    _nodes[16]._northDistance = RED_WEIGHT;

    // (3, 4)
    _nodes[17]._eastDistance = RED_WEIGHT;
    _nodes[17]._westDistance = NORMAL_WEIGHT;
    _nodes[17]._southDistance = NORMAL_WEIGHT;

    // (3, 5)
    _nodes[18]._westDistance = RED_WEIGHT;
    _nodes[18]._southDistance = RED_WEIGHT;
    _nodes[18]._northDistance = RED_WEIGHT;

    // (3, 6)
    _nodes[19]._southDistance = NORMAL_WEIGHT;
    _nodes[19]._northDistance = NORMAL_WEIGHT;

    // (3, 7)
    _nodes[20]._southDistance = NORMAL_WEIGHT;
    _nodes[20]._northDistance = NORMAL_WEIGHT;

    // (4, 1)
    _nodes[21]._eastDistance = NORMAL_WEIGHT;
    _nodes[21]._northDistance = RED_WEIGHT;

    // (4, 2)
    _nodes[22]._eastDistance = NORMAL_WEIGHT;
    _nodes[22]._westDistance = NORMAL_WEIGHT;

    // (4, 3)
    _nodes[23]._westDistance = NORMAL_WEIGHT;
    _nodes[23]._northDistance = NORMAL_WEIGHT;

    // (4, 4)
    _nodes[24]._eastDistance = YELLOW_WEIGHT;
    _nodes[24]._northDistance = NORMAL_WEIGHT;

    // (4, 5)
    _nodes[25]._eastDistance = RED_WEIGHT;
    _nodes[25]._westDistance = YELLOW_WEIGHT;
    _nodes[25]._northDistance = RED_WEIGHT;

    // (4, 6)
    _nodes[26]._eastDistance = NORMAL_WEIGHT;
    _nodes[26]._westDistance = RED_WEIGHT;
    _nodes[26]._northDistance = NORMAL_WEIGHT;

    // (4, 7)
    _nodes[27]._westDistance = NORMAL_WEIGHT;
    _nodes[27]._northDistance = NORMAL_WEIGHT;
}