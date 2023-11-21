#include "pathfindingMode.h"

PathfindingMode::PathfindingMode() : _pathfinder(Pathfinder()),
                                     _x(0), _y(0),
                                     _direction(Direction::SOUTH),
                                     _navigation(MasterNavigation()) {}

void PathfindingMode::run(uint8_t line, uint8_t column)
{
    MovementCode moves[Pathfinder::MAX_PATH_LENGTH];

    for (uint8_t i = 0; i < Pathfinder::MAX_PATH_LENGTH; i++)
        moves[i] = MovementCode::NOTHING;

    pathfind(line, column, moves);
    travelPath(moves);
}

void PathfindingMode::pathfind(uint8_t line, uint8_t column, MovementCode *moves)
{
    uint8_t start = _y * Map::MAP_WIDTH + _x;
    uint8_t dest = line * Map::MAP_WIDTH + column;

    uint8_t path[Pathfinder::MAX_PATH_LENGTH];

    _pathfinder.findPath(start, dest, path);

    processPath(path, moves);
}

void PathfindingMode::travelPath(MovementCode *moves)
{
    // todo, execute the movementcodes. Probably a method of the driving system.
    _navigation.executeMovementCodes(moves, 1);

    _navigation.stop();

    // update the position and orientation. check for a pipe collision
}

void PathfindingMode::processPath(uint8_t *path, MovementCode *moves)
{
    uint8_t index = 0;
    Direction currentDir = _direction;

    while (index < Pathfinder::MAX_PATH_LENGTH - 1 && path[index + 1] != Map::NONE)
    {
        // forward case
        if ((currentDir == Direction::NORTH && path[index + 1] == Map::getNorthPosition(path[index])) ||
            (currentDir == Direction::SOUTH && path[index + 1] == Map::getSouthPosition(path[index])) ||
            (currentDir == Direction::EAST && path[index + 1] == Map::getEastPosition(path[index])) ||
            (currentDir == Direction::WEST && path[index + 1] == Map::getWestPosition(path[index])))
        {
            moves[index++] = MovementCode::FORWARD;
        }
        // left case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getEastPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getNorthPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getSouthPosition(path[index])))
        {
            moves[index++] = MovementCode::LEFT_FORWARD;
        }
        // right case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getEastPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getSouthPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getNorthPosition(path[index])))
        {
            moves[index++] = MovementCode::RIGHT_FORWARD;
        }
        // uturn case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getSouthPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getNorthPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getEastPosition(path[index])))
        {
            moves[index++] = MovementCode::UTURN_FORWARD;
        }

        currentDir = updateOrientation(moves[index - 1], currentDir);
    }

    for (uint8_t i = 1; i < Pathfinder::MAX_PATH_LENGTH; i++)
    {
        if (moves[i] == MovementCode::FORWARD && moves[i - 1] == MovementCode::FORWARD)
        {
            moves[i - 1] = MovementCode::FORWARD_1;
        }
    }
}

void PathfindingMode::updatePosition(MovementCode move)
{
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

        if (static_cast<uint8_t>(currentDir) <= 2)
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
}
