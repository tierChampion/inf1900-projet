#include "robot.h"

Robot::Robot() :
    _pathMode(),
    _cornerMode(),
    _navigation(),
    _piezo()
{
}

void Robot::runPathfindingMode(uint8_t line, uint8_t column)
{
    _pathMode.run(line, column, &_navigation, &_piezo);
}

const char* Robot::runCornerMode()
{
    return _cornerMode.run(&_navigation, &_piezo);
}
