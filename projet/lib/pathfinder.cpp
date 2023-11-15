#include "pathfinder.h"

WorkNode::WorkNode() : _distance(Map::NONE),
                       _travelSettings(0x1F) {}

Visited WorkNode::getVisited() const
{
    return static_cast<Visited>(_travelSettings >> 5);
}

uint8_t WorkNode::getPrev() const
{
    return _travelSettings & 0x1F;
}

void WorkNode::setVisited(Visited visited)
{
    _travelSettings = (_travelSettings & 0x1F) |
                   (static_cast<uint8_t>(visited) << 5);
}

void WorkNode::setPrev(uint8_t prevPos)
{
    _travelSettings = (_travelSettings & 0xE0) |
                   (prevPos & 0x1F);
}

Pathfinder::Pathfinder() : _workSize(0) {}

void Pathfinder::findPath(const Map &map, uint8_t start, uint8_t dest)
{
    _workMap[start]._distance = 0;
    _workMap[start].setVisited(Visited::VISITED);

    _workArray[_workSize++] = start;

    bool finished = false;

    while (!finished && _workSize > 0)
    {
        uint8_t position = popBestNode();

        if (position == dest)
        {
            finished = true;
            continue;
        }

        _workMap[position].setVisited(Visited::FINISHED);
        const MapNode mapNode = map.getNode(position);

        if (mapNode.getCardinalDist(Direction::NORTH) > Map::DISCONNECTED)
        {
            handleNeighbor(position, Map::getNorthPosition(position), mapNode.getCardinalDist(Direction::NORTH));
        }
        if (mapNode.getCardinalDist(Direction::SOUTH) > Map::DISCONNECTED)
        {
            handleNeighbor(position, Map::getSouthPosition(position), mapNode.getCardinalDist(Direction::SOUTH));
        }
        if (mapNode.getCardinalDist(Direction::EAST) > Map::DISCONNECTED)
        {
            handleNeighbor(position, Map::getEastPosition(position), mapNode.getCardinalDist(Direction::EAST));
        }
        if (mapNode.getCardinalDist(Direction::WEST) > Map::DISCONNECTED)
        {
            handleNeighbor(position, Map::getWestPosition(position), mapNode.getCardinalDist(Direction::WEST));
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
    uint8_t minPosition = _workArray[minId];
    uint8_t min = _workMap[minPosition]._distance;
    id++;

    while (id < _workSize)
    {
        if (_workMap[_workArray[id]].getVisited() == Visited::VISITED &&
            _workMap[_workArray[id]]._distance < min)
        {
            minId = id;
            minPosition = _workArray[minId];
            min = _workMap[_workArray[id]]._distance;
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
    uint8_t distance = _workMap[position]._distance +
                       weight;

    if (_workMap[neighborPos].getVisited() != Visited::FINISHED &&
        (distance < _workMap[neighborPos]._distance ||
         _workMap[neighborPos]._distance == Map::NONE))
    {
        _workMap[neighborPos].setPrev(position);
        _workMap[neighborPos]._distance = distance;

        if (_workMap[neighborPos].getVisited() == Visited::UNKNOWN)
        {
            _workArray[_workSize] = neighborPos;
            _workSize++;
        }

        _workMap[neighborPos].setVisited(Visited::VISITED);
    }
}

void Pathfinder::printInvertedPath(uint8_t dest) const
{
    WorkNode node = _workMap[dest];
    PRINT("Inverted path:");
    PRINT(dest);
    while (node.getPrev() != 0x1F)
    {
        PRINT(node.getPrev());
        node = _workMap[node.getPrev()];
    }
}
