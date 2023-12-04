#include "robot.h"

const uint16_t START_DELAY = 2000;

Robot::Robot() : _pathMode(),
                 _cornerMode(),
                 _navigation(),
                 _piezo()
{
}

void Robot::runPathfindingMode(uint8_t line, uint8_t column)
{
    _delay_ms(START_DELAY);
    _pathMode.run(line, column, &_navigation, &_piezo);
}

const char *Robot::runCornerMode()
{
    _delay_ms(START_DELAY);
    return _cornerMode.run(&_navigation, &_piezo);
}
