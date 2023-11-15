#include "pathfinder.h"

Pathfinder::Pathfinder() : _workSize(0) {}

void Pathfinder::findPath(const Map &map, uint8_t start, uint8_t dest)
{
    _workMap[start]._distance = 0;
    _workMap[start]._visited = VisitedState::VISITED;

    _workArray[_workSize++] = start;

    bool finished = false;

    while (!finished && _workSize > 0)
    {
        uint8_t index = popBestNode();

        //PRINT("POPPED:");
        PRINT(index);

        if (index == dest)
        {
            finished = true;
            continue;
        }

        _workMap[index]._visited = VisitedState::FINISHED;
        const MapNode mapNode = map.getNode(index);

        if (mapNode.getCardinalDist(Direction::NORTH) > 0)
        {
            //PRINT("N");
            handleNeighbor(index, getNorthId(index), mapNode.getCardinalDist(Direction::NORTH));
        }
        if (mapNode.getCardinalDist(Direction::SOUTH) > 0)
        {
            //PRINT("S");
            handleNeighbor(index, getSouthId(index), mapNode.getCardinalDist(Direction::SOUTH));
        }
        if (mapNode.getCardinalDist(Direction::EAST) > 0)
        {
            //PRINT("E");
            handleNeighbor(index, getEastId(index), mapNode.getCardinalDist(Direction::EAST));
        }
        if (mapNode.getCardinalDist(Direction::WEST) > 0)
        {
            //PRINT("W");
            handleNeighbor(index, getWestId(index), mapNode.getCardinalDist(Direction::WEST));
        }
    }

#ifdef DEBUG
    printInvertedPath(dest);
#endif
}

uint8_t Pathfinder::popBestNode()
{
    uint8_t id = 0;
    uint8_t minId = id;
    uint8_t minMapId = _workArray[minId];
    uint8_t min = _workMap[minMapId]._distance;
    id++;

    while (id < _workSize)
    {
        if (_workMap[_workArray[id]]._visited == VisitedState::VISITED &&
            _workMap[_workArray[id]]._distance < min)
        {
            minId = id;
            minMapId = _workArray[minId];
            min = _workMap[_workArray[id]]._distance;
        }
        id++;
    }
    _workSize--;

    _workArray[minId] = _workArray[_workSize];
    _workArray[_workSize] = minMapId;

    return minMapId;
}

void Pathfinder::handleNeighbor(uint8_t position, uint8_t neighborPos, uint8_t weight)
{
    uint8_t distance = _workMap[position]._distance +
                       weight;

    if (_workMap[neighborPos]._visited != VisitedState::FINISHED &&
        (distance < _workMap[neighborPos]._distance ||
         _workMap[neighborPos]._distance == 0xFF))
    {
        _workMap[neighborPos]._prev = position;
        _workMap[neighborPos]._distance = distance;

        if (_workMap[neighborPos]._visited == VisitedState::UNKNOWN)
        {
            _workArray[_workSize] = neighborPos;
            _workSize++;
        }

        _workMap[neighborPos]._visited = VisitedState::VISITED;
    }
}

void Pathfinder::printInvertedPath(uint8_t dest) const
{
    WorkNode node = _workMap[dest];
    PRINT("Inverted path:");
    PRINT(dest);
    while (node._prev != 0xFF)
    {
        PRINT(node._prev);
        node = _workMap[node._prev];
    }
}
