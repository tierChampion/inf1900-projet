#include "pathfinder.h"

const uint8_t MASK_PREVIOUS_NODE = 0x1F;

Pathfinder::Pathfinder() : _map(Map()), _workSize(0) {}

void Pathfinder::findPath(uint8_t start, uint8_t dest, uint8_t *path)
{
    _map.reset();

    _map[start].setDistance(0);
    _map[start].setVisited(Visited::VISITED);

    _workArray[_workSize++] = start;

    bool finished = false;

    while (!finished && _workSize > 0)
    {
        uint8_t position = popBestNode();

        if (position == dest)
        {
            PRINT("FOUND!");
            finished = true;
            continue;
        }

        MapNode node = _map[position];
        node.setVisited(Visited::FINISHED);

        if (node.getCardinalDist(Direction::NORTH) > DISCONNECTED)
        {
            handleNeighbor(position, Map::getNorthPosition(position), node.getCardinalDist(Direction::NORTH));
        }
        if (node.getCardinalDist(Direction::SOUTH) > DISCONNECTED)
        {
            handleNeighbor(position, Map::getSouthPosition(position), node.getCardinalDist(Direction::SOUTH));
        }
        if (node.getCardinalDist(Direction::EAST) > DISCONNECTED)
        {
            handleNeighbor(position, Map::getEastPosition(position), node.getCardinalDist(Direction::EAST));
        }
        if (node.getCardinalDist(Direction::WEST) > DISCONNECTED)
        {
            handleNeighbor(position, Map::getWestPosition(position), node.getCardinalDist(Direction::WEST));
        }
    }

#ifdef DEBUG
    printInvertedPath(dest);
#endif

    getPath(dest, path);
}

uint8_t Pathfinder::popBestNode()
{
    uint8_t id = 0;
    uint8_t minId = id;
    uint8_t minPosition = _workArray[minId];
    uint8_t min = _map[minPosition].getDistance();
    id++;

    while (id < _workSize)
    {
        if (_map[_workArray[id]].getVisited() == Visited::VISITED &&
            _map[_workArray[id]].getDistance() < min)
        {
            minId = id;
            minPosition = _workArray[minId];
            min = _map[_workArray[id]].getDistance();
        }
        id++;
    }
    _workSize--;

    _workArray[minId] = _workArray[_workSize];
    _workArray[_workSize] = minPosition;

    return minPosition;
}

void Pathfinder::handleNeighbor(uint8_t position, uint8_t neighborPos, uint8_t weight)
{

    uint8_t distance = _map[position].getDistance() +
                       weight;

    if (_map[neighborPos].getVisited() != Visited::FINISHED &&
        (distance < _map[neighborPos].getDistance() ||
         _map[neighborPos].getDistance() == NONE))
    {
        _map[neighborPos].setPreviousNode(position);
        _map[neighborPos].setDistance(distance);

        if (_map[neighborPos].getVisited() == Visited::UNKNOWN)
        {
            _workArray[_workSize] = neighborPos;
            _workSize++;
        }

        _map[neighborPos].setVisited(Visited::VISITED);
    }
}

void Pathfinder::printInvertedPath(uint8_t dest) const
{
    PRINT("INVERTED PATH:");
    MapNode node = _map[dest];

    while (node.getPreviousNode() != MASK_PREVIOUS_NODE)
    {
        PRINT(node.getPreviousNode());
        node = _map[node.getPreviousNode()];
    }
}

void Pathfinder::getPath(uint8_t dest, uint8_t *path) const
{
    uint8_t size = 0;
    MapNode node = _map[dest];

    while (node.getPreviousNode() != 0x1F)
    {
        ++size;
        node = _map[node.getPreviousNode()];
    }

    node = _map[dest];

    for (int8_t i = Pathfinder::MAX_PATH_LENGTH - 1; i >= 0; i--)
    {
        if (i > size)
        {
            path[i] = NONE;
        }

        else if (i == size)
        {
            path[i] = dest;
        }
        else
        {
            path[i] = node.getPreviousNode();
            node = _map[node.getPreviousNode()];
        }
    }
}

void Pathfinder::modifyMap(uint8_t pipe)
{
    _map.placePillar(pipe);
}

void Pathfinder::resetMap()
{
    _map.removePillar();
}
