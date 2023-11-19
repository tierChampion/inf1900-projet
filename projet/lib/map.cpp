#include "map.h"

const uint8_t NORMAL_WEIGHT = 1;
const uint8_t YELLOW_WEIGHT = 2;
const uint8_t RED_WEIGHT = 5;

MapNode::MapNode() : _verticalDistances(Map::DISCONNECTED),
                     _lateralDistances(Map::DISCONNECTED),
                     _distance(Map::NONE),
                     _travelSettings(0x1F) {}

MapNode::MapNode(uint8_t north,
                 uint8_t south,
                 uint8_t east,
                 uint8_t west) : _verticalDistances((north & 0x0F) | ((south & 0x0F) << 4)),
                                 _lateralDistances((east & 0x0F) | ((west & 0x0F) << 4)),
                                 _distance(Map::NONE),
                                 _travelSettings(0x1F) {}

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

    return Map::DISCONNECTED;
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


void MapNode::setDistance(uint8_t newDist) { _distance = newDist; }

uint8_t MapNode::getDistance() { return _distance; }

Visited MapNode::getVisited() const
{
    return static_cast<Visited>(_travelSettings >> 5);
}

uint8_t MapNode::getPrev() const
{
    return _travelSettings & 0x1F;
}

void MapNode::setVisited(Visited visited)
{
    _travelSettings = (_travelSettings & 0x1F) |
                      (static_cast<uint8_t>(visited) << 5);
}

void MapNode::setPrev(uint8_t prevPos)
{
    _travelSettings = (_travelSettings & 0xE0) |
                      (prevPos & 0x1F);
}

Map::Map()
{
    _pillar = NONE;

    initialiseMap();
}

void Map::reset()
{
    for (uint8_t i = 0; i < Map::NODE_COUNT; i++) {
        _nodes[i].setPrev(0x1F);
        _nodes[i].setDistance(Map::NONE);
        _nodes[i].setVisited(Visited::UNKNOWN);
    }
}

const MapNode &Map::operator[](uint8_t position) const
{
    return _nodes[position];
}

MapNode& Map::operator[](uint8_t position) // wrong
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
        _nodes[getNorthPosition(position)].setCardinalDist(Direction::SOUTH, DISCONNECTED);
    }
    if (y < MAP_HEIGHT - 1)
    {
        _nodes[getSouthPosition(position)].setCardinalDist(Direction::NORTH, DISCONNECTED);
    }
    if (x > 0)
    {
        _nodes[getWestPosition(position)].setCardinalDist(Direction::EAST, DISCONNECTED);
    }
    if (x < MAP_WIDTH - 1)
    {
        _nodes[getEastPosition(position)].setCardinalDist(Direction::WEST, DISCONNECTED);
    }
}

void Map::removePillar()
{
    uint8_t x = getPositionX(_pillar);
    uint8_t y = getPositionY(_pillar);

    if (y > 0)
    {
        _nodes[getNorthPosition(_pillar)].setCardinalDist(Direction::SOUTH,
                                                          _nodes[_pillar].getCardinalDist(Direction::NORTH));
    }
    if (y < MAP_HEIGHT - 1)
    {
        _nodes[getSouthPosition(_pillar)].setCardinalDist(Direction::NORTH,
                                                          _nodes[_pillar].getCardinalDist(Direction::SOUTH));
    }
    if (x > 0)
    {
        _nodes[getWestPosition(_pillar)].setCardinalDist(Direction::EAST,
                                                         _nodes[_pillar].getCardinalDist(Direction::WEST));
    }
    if (x < MAP_WIDTH - 1)
    {
        _nodes[getEastPosition(_pillar)].setCardinalDist(Direction::WEST,
                                                         _nodes[_pillar].getCardinalDist(Direction::EAST));
    }

    _pillar = NONE;
}

uint8_t Map::getPositionX(uint8_t position)
{
    return position % Map::MAP_WIDTH;
}

uint8_t Map::getPositionY(uint8_t position)
{
    return position / Map::MAP_WIDTH;
}

uint8_t Map::getNorthPosition(uint8_t position)
{
    if (getPositionY(position) == 0)
        return Map::NONE;
    return position - Map::MAP_WIDTH;
}

uint8_t Map::getSouthPosition(uint8_t position)
{
    if (getPositionY(position) == Map::MAP_HEIGHT - 1)
        return Map::NONE;
    return position + Map::MAP_WIDTH;
}

uint8_t Map::getEastPosition(uint8_t position)
{
    if (getPositionX(position) == Map::MAP_WIDTH - 1)
        return Map::NONE;
    return position + 1;
}

uint8_t Map::getWestPosition(uint8_t position)
{
    if (getPositionX(position) == 0)
        return Map::NONE;
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
    _nodes[0] = MapNode(DISCONNECTED, NORMAL_WEIGHT, NORMAL_WEIGHT, DISCONNECTED);

    // (1, 2)
    _nodes[1] = MapNode(DISCONNECTED, DISCONNECTED, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (1, 3)
    _nodes[2] = MapNode(DISCONNECTED, NORMAL_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (1, 4)
    _nodes[3] = MapNode(DISCONNECTED, NORMAL_WEIGHT, DISCONNECTED, NORMAL_WEIGHT);

    // (1, 5)
    _nodes[4] = MapNode(DISCONNECTED, DISCONNECTED, NORMAL_WEIGHT, DISCONNECTED);

    // (1, 6)
    _nodes[5] = MapNode(DISCONNECTED, YELLOW_WEIGHT, YELLOW_WEIGHT, NORMAL_WEIGHT);

    // (1, 7)
    _nodes[6] = MapNode(DISCONNECTED, NORMAL_WEIGHT, DISCONNECTED, YELLOW_WEIGHT);

    // (2, 1)
    _nodes[7] = MapNode(NORMAL_WEIGHT, DISCONNECTED, RED_WEIGHT, DISCONNECTED);

    // (2, 2)
    _nodes[8] = MapNode(DISCONNECTED, NORMAL_WEIGHT, NORMAL_WEIGHT, RED_WEIGHT);

    // (2, 3)
    _nodes[9] = MapNode(NORMAL_WEIGHT, RED_WEIGHT, DISCONNECTED, NORMAL_WEIGHT);

    // (2, 4)
    _nodes[10] = MapNode(NORMAL_WEIGHT, DISCONNECTED, YELLOW_WEIGHT, DISCONNECTED);

    // (2, 5)
    _nodes[11] = MapNode(DISCONNECTED, RED_WEIGHT, NORMAL_WEIGHT, YELLOW_WEIGHT);

    // (2, 6)
    _nodes[12] = MapNode(YELLOW_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (2, 7)
    _nodes[13] = MapNode(NORMAL_WEIGHT, NORMAL_WEIGHT, DISCONNECTED, NORMAL_WEIGHT);

    // (3, 1)
    _nodes[14] = MapNode(DISCONNECTED, RED_WEIGHT, YELLOW_WEIGHT, DISCONNECTED);

    // (3, 2)
    _nodes[15] = MapNode(NORMAL_WEIGHT, DISCONNECTED, NORMAL_WEIGHT, YELLOW_WEIGHT);

    // (3, 3)
    _nodes[16] = MapNode(RED_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (3, 4)
    _nodes[17] = MapNode(DISCONNECTED, NORMAL_WEIGHT, RED_WEIGHT, NORMAL_WEIGHT);

    // (3, 5)
    _nodes[18] = MapNode(RED_WEIGHT, RED_WEIGHT, DISCONNECTED, RED_WEIGHT);

    // (3, 6)
    _nodes[19] = MapNode(NORMAL_WEIGHT, NORMAL_WEIGHT, DISCONNECTED, DISCONNECTED);

    // (3, 7)
    _nodes[20] = MapNode(NORMAL_WEIGHT, NORMAL_WEIGHT, DISCONNECTED, DISCONNECTED);

    // (4, 1)
    _nodes[21] = MapNode(RED_WEIGHT, DISCONNECTED, NORMAL_WEIGHT, DISCONNECTED);

    // (4, 2)
    _nodes[22] = MapNode(DISCONNECTED, DISCONNECTED, NORMAL_WEIGHT, NORMAL_WEIGHT);

    // (4, 3)
    _nodes[23] = MapNode(NORMAL_WEIGHT, DISCONNECTED, DISCONNECTED, NORMAL_WEIGHT);

    // (4, 4)
    _nodes[24] = MapNode(NORMAL_WEIGHT, DISCONNECTED, YELLOW_WEIGHT, DISCONNECTED);

    // (4, 5)
    _nodes[25] = MapNode(RED_WEIGHT, DISCONNECTED, RED_WEIGHT, YELLOW_WEIGHT);

    // (4, 6)
    _nodes[26] = MapNode(NORMAL_WEIGHT, DISCONNECTED, NORMAL_WEIGHT, RED_WEIGHT);

    // (4, 7)
    _nodes[27] = MapNode(NORMAL_WEIGHT, DISCONNECTED, DISCONNECTED, NORMAL_WEIGHT);
}