#include "pathfinding_mode.h"

/**
 * /fichier map_node.cpp
 * /auteurs Thierry Champion, Nikolai Olekhnovitch, Raisse Oumarou Petitot, Rym Touati
 * /date    05 décembre 2023
 * Créé le 10 novembre
 * 
 * Classe qui représente le mode de la recherche de chemin. Cette classe est aussi responsable
 * de convertir une série de noeud en une série de mouvements.
 * 
 * Les déplacements se font soit par pair de rotation et de déplacement vers l'avant ou uniquement 
 * avec un déplacement vers l'avant. Si jamais un déplacement en ligne droite se terminerait sur le 
 * milieu d'une ligne, alors une ligne droite d'une unité est effectuée.
 */

PathfindingMode::PathfindingMode() : _distSensor(DistanceSensor()),
                                     _pathfinder(Pathfinder()),
                                     _position(0),
                                     _direction(Direction::SOUTH)
{
}

void PathfindingMode::run(uint8_t line, uint8_t column, MasterNavigation *navigation, Piezo *piezo)
{
    MovementCode moves[2 * Pathfinder::MAX_PATH_LENGTH];

    bool pathSuccess = false;
    while (!pathSuccess)
    {
        for (uint8_t i = 0; i < 2 * Pathfinder::MAX_PATH_LENGTH; i++)
            moves[i] = MovementCode::NOTHING;

        pathfind(line, column, moves);
        pathSuccess = travelPath(moves, navigation, piezo);
    }

    finishedPath(piezo);
    _pathfinder.resetMap();
}

void PathfindingMode::pathfind(uint8_t line, uint8_t column, MovementCode *moves)
{
    uint8_t dest = line * MAP_WIDTH + column;

    uint8_t path[Pathfinder::MAX_PATH_LENGTH];

    _pathfinder.findPath(_position, dest, path);

    processPath(path, moves);
}

bool PathfindingMode::travelPath(MovementCode *moves, MasterNavigation *navigation, Piezo *piezo)
{
    uint8_t i = 0;
    while (i < 2 * Pathfinder::MAX_PATH_LENGTH)
    {
        if (moves[i] == MovementCode::FORWARD || moves[i] == MovementCode::FORWARD_1)
        {
            _delay_ms(250);
            if (_distSensor.isClose())
            {
                foundPillar(moves[i], piezo);
                return false;
            }
        }

        bool calibrate = (moves[i] == MovementCode::FORWARD) &&
                         (i == 0 || moves[i - 1] != MovementCode::FORWARD_1);

        navigation->executeMovementCode(moves[i], calibrate);

        _direction = updateOrientation(moves[i], _direction);
        _position = updatePosition(moves[i], _direction, _position);

        i++;
    }

    navigation->stop();
    return true;
}

void PathfindingMode::foundPillar(MovementCode currentMove, Piezo *piezo)
{
    piezo->playFoundPillarMelody();

    _pathfinder.modifyMap(updatePosition(currentMove, _direction, _position));
}

void PathfindingMode::finishedPath(Piezo *piezo, bool beautiful)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (beautiful)
        {
            for (uint8_t j = 0; j < 13; j++)
            {
                piezo->play(75 + i);
                _delay_ms(15.36);
                piezo->play(75 + i + 7);
                _delay_ms(15.36);
            }
        }
        else
        {
            piezo->play(75 + i);
            _delay_ms(200);
        }

        piezo->stop();
        _delay_ms(100);
    }
}

void PathfindingMode::processPath(uint8_t *path, MovementCode *moves)
{
    uint8_t index = 0;
    uint8_t moveIndex = 0;
    Direction currentDir = _direction;
    uint8_t currentPos = _position;

    while (index < Pathfinder::MAX_PATH_LENGTH - 1 && path[index + 1] != NONE)
    {

        PRINT("pos");
        PRINT(path[index]);

        // forward case
        if ((currentDir == Direction::NORTH && path[index + 1] == Map::getNorthPosition(path[index])) ||
            (currentDir == Direction::SOUTH && path[index + 1] == Map::getSouthPosition(path[index])) ||
            (currentDir == Direction::EAST && path[index + 1] == Map::getEastPosition(path[index])) ||
            (currentDir == Direction::WEST && path[index + 1] == Map::getWestPosition(path[index])))
        {
            moves[moveIndex++] = MovementCode::FORWARD;
        }
        // left case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getEastPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getNorthPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getSouthPosition(path[index])))
        {
            moves[moveIndex++] = MovementCode::LEFT;
            currentDir = updateOrientation(moves[moveIndex - 1], currentDir);
            moves[moveIndex++] = MovementCode::FORWARD;
        }
        // right case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getEastPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getSouthPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getNorthPosition(path[index])))
        {
            moves[moveIndex++] = MovementCode::RIGHT;
            currentDir = updateOrientation(moves[moveIndex - 1], currentDir);
            moves[moveIndex++] = MovementCode::FORWARD;
        }
        // uturn case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getSouthPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getNorthPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getEastPosition(path[index])))
        {
            moves[moveIndex++] = MovementCode::UTURN;
            currentDir = updateOrientation(moves[moveIndex - 1], currentDir);
            moves[moveIndex++] = MovementCode::FORWARD;
        }

        // change to forward 1 if the move ends in the middle of a line.
        currentPos = updatePosition(moves[moveIndex - 1], currentDir, currentPos);
        if (_pathfinder.getMap().isLinePosition(currentPos))
        {
            moves[moveIndex - 1] = MovementCode::FORWARD_1;
        }

        index++;
        PRINT(static_cast<uint8_t>(currentDir));
    }

    if (_pathfinder.getMap()[currentPos].getCardinalDist(currentDir) == DISCONNECTED)
    {
        moves[moveIndex++] = MovementCode::UTURN;
    }

#ifdef DEBUG

    PRINT("MOVES:");
    for (uint8_t i = 1; i < 2 * Pathfinder::MAX_PATH_LENGTH; i++)
    {
        PRINT(static_cast<uint8_t>(moves[i - 1]));
    }
#endif
}

uint8_t PathfindingMode::updatePosition(MovementCode move, Direction currentDir, uint8_t currentPos)
{
    if (move != MovementCode::FORWARD && move != MovementCode::FORWARD_1)
    {
        return currentPos;
    }
    else if (currentDir == Direction::NORTH)
    {
        return Map::getNorthPosition(currentPos);
    }
    else if (currentDir == Direction::SOUTH)
    {
        return Map::getSouthPosition(currentPos);
    }
    else if (currentDir == Direction::EAST)
    {
        return Map::getEastPosition(currentPos);
    }
    else if (currentDir == Direction::WEST)
    {
        return Map::getWestPosition(currentPos);
    }

    return currentPos;
}

Direction PathfindingMode::updateOrientation(MovementCode move, Direction currentDir)
{
    switch (move)
    {
    case MovementCode::LEFT:
        currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) / 2) == 1
                                                ? static_cast<uint8_t>(currentDir) - 2
                                                : static_cast<uint8_t>(currentDir) + 2);

        if (static_cast<uint8_t>(currentDir) >= 2)
        {
            currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) % 2) == 1
                                                    ? static_cast<uint8_t>(currentDir) - 1
                                                    : static_cast<uint8_t>(currentDir) + 1);
        }
        return currentDir;
        break;

    case MovementCode::RIGHT:
        currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) / 2) == 1
                                                ? static_cast<uint8_t>(currentDir) - 2
                                                : static_cast<uint8_t>(currentDir) + 2);

        if (static_cast<uint8_t>(currentDir) < 2)
        {
            currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) % 2) == 1
                                                    ? static_cast<uint8_t>(currentDir) - 1
                                                    : static_cast<uint8_t>(currentDir) + 1);
        }
        return currentDir;
        break;

    case MovementCode::UTURN:
        currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) % 2) == 1
                                                ? static_cast<uint8_t>(currentDir) - 1
                                                : static_cast<uint8_t>(currentDir) + 1);
        return currentDir;
        break;

    case MovementCode::FORWARD:
    case MovementCode::FORWARD_1:
    default:
        break;
    }

    return currentDir;
}

void PathfindingMode::setPosition(uint8_t line, uint8_t column, Direction dir)
{
    _position = line * MAP_WIDTH + column;
    _direction = dir;
}
