#include "map_node.h"

/**
 * /fichier map_node.cpp
 * /auteurs Thierry Champion, Nikolai Olekhnovitch, Raisse Oumarou Petitot, Rym Touati
 * /date    05 décembre 2023
 * Créé le 10 novembre
 * 
 * Classe qui représente un noeud sur la carte. Chaque noeud contient 4 octets.
 * Les deux premiers sont utilisées pour les distances nord, sud, est, ouest.
 * Le troisième octet contient la distance depuis le noeud de départ.
 * Finalement, le quatrième octet contient le statut de visité ainsi que l'index 
 *      du noeud précédent.
 * 
 * Mapping: 4 bits pour le sud, 4 bits pour le nord, 4 bits pour l'est, 4 bits pour l'ouest,
 * 8 bits pour la distance, 3 bits pour le statut de visite et 5 bits pour l'index du noeud
 * précédent.
 */

const uint8_t MASK_NORTH_NODE_DISTANCE = 0x0F;
const uint8_t MASK_SOUTH_NODE_DISTANCE = 0xF0;
const uint8_t MASK_EAST_NODE_DISTANCE = 0x0F;
const uint8_t MASK_WEST_NODE_DISTANCE = 0xF0;
const uint8_t SOUTH_WEST_INDEX = 4;
const uint8_t VISITED_INDEX = 5;
const uint8_t MASK_PREVIOUS_NODE = 0x1F;
const uint8_t MASK_NODE_STATE = 0xE0;

MapNode::MapNode() : _verticalDistances(DISCONNECTED),
                     _lateralDistances(DISCONNECTED),
                     _distance(NONE),
                     _travelSettings(0x1F) {}

MapNode::MapNode(uint8_t north,
                 uint8_t south,
                 uint8_t east,
                 uint8_t west) : _verticalDistances((north & MASK_NORTH_NODE_DISTANCE) | 
                 ((south << SOUTH_WEST_INDEX) & MASK_SOUTH_NODE_DISTANCE)),
                                 _lateralDistances((east & MASK_EAST_NODE_DISTANCE) | 
                                 ((west << SOUTH_WEST_INDEX) & MASK_WEST_NODE_DISTANCE)),
                                 _distance(NONE),
                                 _travelSettings(MASK_PREVIOUS_NODE) {}

uint8_t MapNode::getCardinalDist(Direction direction) const
{
    switch (direction)
    {
        case Direction::NORTH:
            return _verticalDistances & MASK_NORTH_NODE_DISTANCE;
            break;
        case Direction::SOUTH:
            return (_verticalDistances & MASK_SOUTH_NODE_DISTANCE) >> SOUTH_WEST_INDEX;
            break;
        case Direction::EAST:
            return _lateralDistances & MASK_EAST_NODE_DISTANCE;
            break;
        case Direction::WEST:
            return (_lateralDistances & MASK_WEST_NODE_DISTANCE) >> SOUTH_WEST_INDEX;
            break;
        default:
            break;
    }

    return DISCONNECTED;
}

void MapNode::setCardinalDist(Direction direction, uint8_t newDistance)
{
    switch (direction)
    {
        case Direction::NORTH:
            _verticalDistances = (_verticalDistances & MASK_SOUTH_NODE_DISTANCE) | (newDistance & MASK_NORTH_NODE_DISTANCE);
            break;
        case Direction::SOUTH:
            _verticalDistances = (_verticalDistances & MASK_NORTH_NODE_DISTANCE) | 
            ((newDistance << SOUTH_WEST_INDEX) & MASK_SOUTH_NODE_DISTANCE);
            break;
        case Direction::EAST:
            _lateralDistances = (_lateralDistances & MASK_WEST_NODE_DISTANCE) | (newDistance & MASK_EAST_NODE_DISTANCE);
            break;
        case Direction::WEST:
            _lateralDistances = (_lateralDistances & MASK_EAST_NODE_DISTANCE) | 
            ((newDistance << SOUTH_WEST_INDEX) & MASK_WEST_NODE_DISTANCE);
            break;
    }
}

void MapNode::setDistance(uint8_t newDistance) { _distance = newDistance; }

uint8_t MapNode::getDistance() { return _distance; }

Visited MapNode::getVisited() const
{
    return static_cast<Visited>(_travelSettings >> VISITED_INDEX);
}

uint8_t MapNode::getPreviousNode() const
{
    return _travelSettings & MASK_PREVIOUS_NODE;
}

void MapNode::setVisited(Visited visited)
{
    _travelSettings = (_travelSettings & MASK_PREVIOUS_NODE) |
                      (static_cast<uint8_t>(visited) << VISITED_INDEX);
}

void MapNode::setPreviousNode(uint8_t previousPosition)
{
    _travelSettings = (_travelSettings & MASK_NODE_STATE) |
                      (previousPosition & MASK_PREVIOUS_NODE);
}