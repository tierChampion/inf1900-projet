#include "map.h"

const uint8_t NONE = 0xFF;
const uint8_t NORMAL_WEIGHT = 1;
const uint8_t YELLOW_WEIGHT = 2;
const uint8_t RED_WEIGHT = 5;

MapNode::MapNode(uint8_t north,
                 uint8_t south,
                 uint8_t east,
                 uint8_t west) : _verticalDistances((north & 0x0F) | ((south & 0x0F) << 4)),
                                 _lateralDistances((east & 0x0F) | ((west & 0x0F) << 4))
{
}

uint8_t MapNode::getCardinalDist(Direction direction) const
{
    switch (direction)
    {
    case Direction::NORTH:
        return _verticalDistances & 0x0F;
        break;
    case Direction::SOUTH:
        return (_verticalDistances & 0xF0) >> 4;
        break;
    case Direction::EAST:
        return _lateralDistances & 0x0F;
        break;
    case Direction::WEST:
        return (_lateralDistances & 0xF0) >> 4;
        break;
    }
}

void MapNode::setCardinalDist(Direction direction, uint8_t newDist)
{
    switch (direction)
    {
    case Direction::NORTH:
        _verticalDistances = (_verticalDistances & 0xF0) | (newDist & 0x0F);
        break;
    case Direction::SOUTH:
        _verticalDistances = (_verticalDistances & 0x0F) | (newDist & 0xF0);
        break;
    case Direction::EAST:
        _lateralDistances = (_lateralDistances & 0xF0) | (newDist & 0x0F);
        break;
    case Direction::WEST:
        _lateralDistances = (_lateralDistances & 0x0F) | (newDist & 0xF0);
        break;
    }
}

Map::Map()
{
    _pillar = NONE;

    initialiseMap();
}

const MapNode &Map::getNode(uint8_t position) const
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
        _nodes[getNorthId(position)].setCardinalDist(Direction::SOUTH, 0);
    }
    if (y < MAP_HEIGHT - 1)
    {
        _nodes[getSouthId(position)].setCardinalDist(Direction::NORTH, 0);
    }
    if (x > 0)
    {
        _nodes[getWestId(position)].setCardinalDist(Direction::EAST, 0);
    }
    if (x < MAP_WIDTH - 1)
    {
        _nodes[getEastId(position)].setCardinalDist(Direction::WEST, 0);
    }
}

void Map::removePillar()
{
    uint8_t x = getPositionX(_pillar);
    uint8_t y = getPositionY(_pillar);

    if (y > 0)
    {
        _nodes[getNorthId(_pillar)].setCardinalDist(Direction::SOUTH,
                                                    _nodes[_pillar].getCardinalDist(Direction::NORTH));
    }
    if (y < MAP_HEIGHT - 1)
    {
        _nodes[getSouthId(_pillar)].setCardinalDist(Direction::NORTH,
                                                    _nodes[_pillar].getCardinalDist(Direction::SOUTH));
    }
    if (x > 0)
    {
        _nodes[getWestId(_pillar)].setCardinalDist(Direction::EAST,
                                                   _nodes[_pillar].getCardinalDist(Direction::WEST));
    }
    if (x < MAP_WIDTH - 1)
    {
        _nodes[getEastId(_pillar)].setCardinalDist(Direction::WEST,
                                                   _nodes[_pillar].getCardinalDist(Direction::EAST));
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
        PRINT(_nodes[i].getCardinalDist(Direction::NORTH));
        PRINT(_nodes[i].getCardinalDist(Direction::SOUTH));
        PRINT(_nodes[i].getCardinalDist(Direction::EAST));
        PRINT(_nodes[i].getCardinalDist(Direction::WEST));
    }
}

void Map::initialiseMap()
{
    // (1, 1)
    _nodes[0] = MapNode(0, NORMAL_WEIGHT, NORMAL_WEIGHT, 0);

    // (1, 2)
    _nodes[1] = MapNode(0, 0, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (1, 3)
    _nodes[2] = MapNode(0, NORMAL_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (1, 4)
    _nodes[3] = MapNode(0, NORMAL_WEIGHT, 0, NORMAL_WEIGHT);

    // (1, 5)
    _nodes[4] = MapNode(0, 0, NORMAL_WEIGHT, 0);

    // (1, 6)
    _nodes[5] = MapNode(0, YELLOW_WEIGHT, YELLOW_WEIGHT, NORMAL_WEIGHT);

    // (1, 7)
    _nodes[6] = MapNode(0, NORMAL_WEIGHT, 0, YELLOW_WEIGHT);

    // (2, 1)
    _nodes[7] = MapNode(NORMAL_WEIGHT, 0, RED_WEIGHT, 0);

    // (2, 2)
    _nodes[8] = MapNode(0, NORMAL_WEIGHT, NORMAL_WEIGHT, RED_WEIGHT);

    // (2, 3)
    _nodes[9] = MapNode(NORMAL_WEIGHT, RED_WEIGHT, 0, NORMAL_WEIGHT);

    // (2, 4)
    _nodes[10] = MapNode(NORMAL_WEIGHT, 0, YELLOW_WEIGHT, 0);

    // (2, 5)
    _nodes[11] = MapNode(0, RED_WEIGHT, NORMAL_WEIGHT, YELLOW_WEIGHT);

    // (2, 6)
    _nodes[12] = MapNode(YELLOW_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (2, 7)
    _nodes[13] = MapNode(NORMAL_WEIGHT, NORMAL_WEIGHT, 0, NORMAL_WEIGHT);

    // (3, 1)
    _nodes[14] = MapNode(0, RED_WEIGHT, YELLOW_WEIGHT, 0);

    // (3, 2)
    _nodes[15] = MapNode(NORMAL_WEIGHT, 0, NORMAL_WEIGHT, YELLOW_WEIGHT);

    // (3, 3)
    _nodes[16] = MapNode(RED_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (3, 4)
    _nodes[17] = MapNode(0, NORMAL_WEIGHT, RED_WEIGHT, NORMAL_WEIGHT);

    // (3, 5)
    _nodes[18] = MapNode(RED_WEIGHT, RED_WEIGHT, 0, RED_WEIGHT);

    // (3, 6)
    _nodes[19] = MapNode(NORMAL_WEIGHT, NORMAL_WEIGHT, 0, 0);

    // (3, 7)
    _nodes[20] = MapNode(NORMAL_WEIGHT, NORMAL_WEIGHT, 0, 0);

    // (4, 1)
    _nodes[21] = MapNode(RED_WEIGHT, 0, NORMAL_WEIGHT, 0);

    // (4, 2)
    _nodes[22] = MapNode(0, 0, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (4, 3)
    _nodes[23] = MapNode(NORMAL_WEIGHT, 0, 0, NORMAL_WEIGHT);

    // (4, 4)
    _nodes[24] = MapNode(NORMAL_WEIGHT, 0, YELLOW_WEIGHT, 0);

    // (4, 5)
    _nodes[25] = MapNode(RED_WEIGHT, 0, RED_WEIGHT, YELLOW_WEIGHT);

    // (4, 6)
    _nodes[26] = MapNode(NORMAL_WEIGHT, 0, NORMAL_WEIGHT, RED_WEIGHT);

    // (4, 7)
    _nodes[27] = MapNode(NORMAL_WEIGHT, 0, 0, NORMAL_WEIGHT);
}