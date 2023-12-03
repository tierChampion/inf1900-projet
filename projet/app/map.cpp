#include "map.h"
const uint8_t NORMAL_WEIGHT = 1;
const uint8_t YELLOW_WEIGHT = 2;
const uint8_t RED_WEIGHT = 5;

const uint8_t MASK_PREVIOUS_NODE = 0x1F;

Map::Map()
{
    _pillar = NONE;

    initialiseMap();
}

void Map::reset()
{
    for (uint8_t i = 0; i < NODE_COUNT; i++)
    {
        _nodes[i].setPreviousNode(MASK_PREVIOUS_NODE);
        _nodes[i].setDistance(NONE);
        _nodes[i].setVisited(Visited::UNKNOWN);
    }
}

const MapNode &Map::operator[](uint8_t position) const
{
    return _nodes[position];
}

MapNode &Map::operator[](uint8_t position)
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

    PRINT("PILLAR POSITION");
    PRINT(_pillar);
    PRINT(x);
    PRINT(y);

    PRINT("PILLAR NEIGHBORS");
    PRINT(getNorthPosition(_pillar));
    PRINT(getSouthPosition(_pillar));
    PRINT(getWestPosition(_pillar));
    PRINT(getEastPosition(_pillar));
    PRINT("NEIGHBORS DISTANCE");
    PRINT(_nodes[getNorthPosition(_pillar)].getCardinalDist(Direction::SOUTH));
    PRINT(_nodes[getSouthPosition(_pillar)].getCardinalDist(Direction::NORTH));
    PRINT(_nodes[getWestPosition(_pillar)].getCardinalDist(Direction::EAST));
    PRINT(_nodes[getEastPosition(_pillar)].getCardinalDist(Direction::WEST));
    PRINT("PILLAR DISTANCE");
    PRINT(_nodes[_pillar].getCardinalDist(Direction::NORTH));
    PRINT(_nodes[_pillar].getCardinalDist(Direction::SOUTH));
    PRINT(_nodes[_pillar].getCardinalDist(Direction::WEST));
    PRINT(_nodes[_pillar].getCardinalDist(Direction::EAST));

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

    PRINT("FINAL NEIGHBORS DISTANCE");
    PRINT(_nodes[getNorthPosition(_pillar)].getCardinalDist(Direction::SOUTH));
    PRINT(_nodes[getSouthPosition(_pillar)].getCardinalDist(Direction::NORTH));
    PRINT(_nodes[getWestPosition(_pillar)].getCardinalDist(Direction::EAST));
    PRINT(_nodes[getEastPosition(_pillar)].getCardinalDist(Direction::WEST));

    _pillar = NONE;
}

uint8_t Map::getPositionX(uint8_t position)
{
    return position % MAP_WIDTH;
}

uint8_t Map::getPositionY(uint8_t position)
{
    return position / MAP_WIDTH;
}

uint8_t Map::getNorthPosition(uint8_t position)
{
    if (getPositionY(position) == 0)
        return NONE;
    return position - MAP_WIDTH;
}

uint8_t Map::getSouthPosition(uint8_t position)
{
    if (getPositionY(position) == MAP_HEIGHT - 1)
        return NONE;
    return position + MAP_WIDTH;
}

uint8_t Map::getEastPosition(uint8_t position)
{
    if (getPositionX(position) == MAP_WIDTH - 1)
        return NONE;
    return position + 1;
}

uint8_t Map::getWestPosition(uint8_t position)
{
    if (getPositionX(position) == 0)
        return NONE;
    return position - 1;
}

bool Map::isLinePosition(uint8_t position)
{
    return ((_nodes[position].getCardinalDist(Direction::NORTH) > 0 ||
             _nodes[position].getCardinalDist(Direction::SOUTH) > 0) &&
            (_nodes[position].getCardinalDist(Direction::EAST) == 0 &&
             _nodes[position].getCardinalDist(Direction::WEST) == 0)) ||
           ((_nodes[position].getCardinalDist(Direction::EAST) > 0 ||
             _nodes[position].getCardinalDist(Direction::WEST) > 0) &&
            (_nodes[position].getCardinalDist(Direction::NORTH) == 0 &&
             _nodes[position].getCardinalDist(Direction::SOUTH) == 0));
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