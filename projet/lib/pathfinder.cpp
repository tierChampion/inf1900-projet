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

        if (index == dest)
        {
            finished = true;
            continue;
        }

        _workMap[index]._visited = VisitedState::FINISHED;
        const MapNode mapNode = map.getNode(index);

        if (mapNode._northDistance > 0)
        {
            handleNeighbor(index, getNorthId(index), mapNode._northDistance);
        }
        if (mapNode._southDistance > 0)
        {
            handleNeighbor(index, getSouthId(index), mapNode._southDistance);
        }
        if (mapNode._eastDistance > 0)
        {
            handleNeighbor(index, getEastId(index), mapNode._eastDistance);
        }
        if (mapNode._westDistance > 0)
        {
            handleNeighbor(index, getWestId(index), mapNode._westDistance);
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

    uint8_t temp = minId;
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
