#include "pathfinding_mode.h"

PathfindingMode::PathfindingMode() : _navigation(MasterNavigation()),
                                     _distSensor(DistanceSensor()),
                                     _pathfinder(Pathfinder()),
                                     _piezo(Piezo()),
                                     _position(0),
                                     _direction(Direction::SOUTH)
{
}

void PathfindingMode::run(uint8_t line, uint8_t column)
{
    MovementCode moves[2 * Pathfinder::MAX_PATH_LENGTH];

    _pathfinder.resetMap();

    bool pathSuccess = false;
    while (!pathSuccess)
    {
        for (uint8_t i = 0; i < 2 * Pathfinder::MAX_PATH_LENGTH; i++)
            moves[i] = MovementCode::NOTHING;

        pathfind(line, column, moves);
        pathSuccess = travelPath(moves);
    }

    finishedPath();
}

void PathfindingMode::pathfind(uint8_t line, uint8_t column, MovementCode *moves)
{
    uint8_t dest = line * Map::MAP_WIDTH + column;

    uint8_t path[Pathfinder::MAX_PATH_LENGTH];

    bool isDestInMiddle = _pathfinder.findPath(_position, dest, path);

    processPath(path, isDestInMiddle, moves);
}

bool PathfindingMode::travelPath(MovementCode *moves)
{
    uint8_t i = 0;
    while (i < 2 * Pathfinder::MAX_PATH_LENGTH)
    {
        if (moves[i] == MovementCode::FORWARD || moves[i] == MovementCode::FORWARD_1)
        {
            if (_distSensor.isClose())
            {
                foundPillar(moves[i]);
                return false;
            }
        }

        _navigation.executeMovementCode(moves[i]);

        _direction = updateOrientation(moves[i], _direction);
        _position = updatePosition(moves[i], _direction, _position);

        i++;
    }

    _navigation.stop();
    return true;
}

void PathfindingMode::foundPillar(MovementCode currentMove)
{
    _piezo.play(45);
    _delay_ms(2000);
    _piezo.stop();

    _pathfinder.modifyMap(updatePosition(currentMove, _direction, _position));
}

void PathfindingMode::finishedPath()
{
    for (uint8_t i = 0; i < 5; i++) {
        _piezo.play(75 + i);
        _delay_ms(200);

        _piezo.stop();
        _delay_ms(100);
    }
}

void PathfindingMode::processPath(uint8_t *path, bool isDestMiddle, MovementCode *moves)
{
    uint8_t index = 0;
    uint8_t moveIndex = 0;
    Direction currentDir = _direction;

    while (index < Pathfinder::MAX_PATH_LENGTH - 1 && path[index + 1] != Map::NONE)
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
        index++;
        PRINT(static_cast<uint8_t>(currentDir));
    }

    PRINT("MOVES:");

    for (uint8_t i = 1; i < 2 * Pathfinder::MAX_PATH_LENGTH; i++)
    {
        if (moves[i] == MovementCode::FORWARD && moves[i - 1] == MovementCode::FORWARD)
        {
            moves[i - 1] = MovementCode::FORWARD_1;
        }
        else if (moves[i] == MovementCode::NOTHING && moves[i - 1] == MovementCode::FORWARD && isDestMiddle)
        {
            moves[i - 1] = MovementCode::FORWARD_1;
        }

        PRINT(static_cast<uint8_t>(moves[i - 1]));
    }
}

uint8_t PathfindingMode::updatePosition(MovementCode move, Direction currentDir, uint8_t currentPos)
{
    if ((currentDir == Direction::NORTH && (move == MovementCode::FORWARD || move == MovementCode::FORWARD_1)) ||
        (currentDir == Direction::SOUTH && (move == MovementCode::UTURN_FORWARD)) ||
        (currentDir == Direction::EAST && (move == MovementCode::LEFT_FORWARD)) ||
        (currentDir == Direction::WEST && (move == MovementCode::RIGHT_FORWARD)))
    {
        return Map::getNorthPosition(currentPos);
    }
    else if ((currentDir == Direction::SOUTH && (move == MovementCode::FORWARD || move == MovementCode::FORWARD_1)) ||
             (currentDir == Direction::NORTH && (move == MovementCode::UTURN_FORWARD)) ||
             (currentDir == Direction::WEST && (move == MovementCode::LEFT_FORWARD)) ||
             (currentDir == Direction::EAST && (move == MovementCode::RIGHT_FORWARD)))
    {
        return Map::getSouthPosition(currentPos);
    }
    else if ((currentDir == Direction::EAST && (move == MovementCode::FORWARD || move == MovementCode::FORWARD_1)) ||
             (currentDir == Direction::WEST && (move == MovementCode::UTURN_FORWARD)) ||
             (currentDir == Direction::SOUTH && (move == MovementCode::LEFT_FORWARD)) ||
             (currentDir == Direction::NORTH && (move == MovementCode::RIGHT_FORWARD)))
    {
        return Map::getEastPosition(currentPos);
    }
    else if ((currentDir == Direction::WEST && (move == MovementCode::FORWARD || move == MovementCode::FORWARD_1)) ||
             (currentDir == Direction::EAST && (move == MovementCode::UTURN_FORWARD)) ||
             (currentDir == Direction::NORTH && (move == MovementCode::LEFT_FORWARD)) ||
             (currentDir == Direction::SOUTH && (move == MovementCode::RIGHT_FORWARD)))
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
    case MovementCode::LEFT_FORWARD:
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
    case MovementCode::RIGHT_FORWARD:
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
    case MovementCode::UTURN_FORWARD:
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
