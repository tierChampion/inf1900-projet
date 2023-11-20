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
            // 0 - 3 - 1 - 2
            currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) / 2) == 1
                                                    ? static_cast<uint8_t>(currentDir) - 2
                                                    : static_cast<uint8_t>(currentDir) + 2);

            if (static_cast<uint8_t>(currentDir) >= 2)
            {
                currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) % 2) == 1
                                                        ? static_cast<uint8_t>(currentDir) - 1
                                                        : static_cast<uint8_t>(currentDir) + 1);
            }
        }
        // right case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getEastPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getSouthPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getNorthPosition(path[index])))
        {
            moves[index++] = MovementCode::RIGHT_FORWARD;
            // 0 - 2 - 1 - 3
            currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) / 2) == 1
                                                    ? static_cast<uint8_t>(currentDir) - 2
                                                    : static_cast<uint8_t>(currentDir) + 2);

            if (static_cast<uint8_t>(currentDir) <= 2)
            {
                currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) % 2) == 1
                                                        ? static_cast<uint8_t>(currentDir) - 1
                                                        : static_cast<uint8_t>(currentDir) + 1);
            }
        }
        // uturn case
        else if ((currentDir == Direction::NORTH && path[index + 1] == Map::getSouthPosition(path[index])) ||
                 (currentDir == Direction::SOUTH && path[index + 1] == Map::getNorthPosition(path[index])) ||
                 (currentDir == Direction::EAST && path[index + 1] == Map::getWestPosition(path[index])) ||
                 (currentDir == Direction::WEST && path[index + 1] == Map::getEastPosition(path[index])))
        {
            moves[index++] = MovementCode::UTURN_FORWARD;
            // 0 -- 1 2 -- 3
            currentDir = static_cast<Direction>((static_cast<uint8_t>(currentDir) % 2) == 1
                                                    ? static_cast<uint8_t>(currentDir) - 1
                                                    : static_cast<uint8_t>(currentDir) + 1);
        }

        PRINT("MOVE:");
        PRINT(static_cast<uint8_t>(moves[index - 1]));
        PRINT(static_cast<uint8_t>(currentDir));
        PRINT(static_cast<uint8_t>(path[index]));
        PRINT(static_cast<uint8_t>(path[index + 1]));
    }
}

void PathfindingMode::travelPath(MovementCode *moves)
{
    // todo, execute the movementcodes. Probably a method of the driving system.
    _navigation.executeMovementCodes(moves, 1);

    _navigation.stop();
}
