#ifndef ROBOT_H
#define ROBOT_H

#include "pathfinding_mode.h"
#include "corners_detector.h"
#include "master_navigation.h"
#include "piezo.h"

class Robot
{
public:
    Robot();

    void runPathfindingMode(uint8_t line, uint8_t column);
    const char* runCornerMode();

private:
    PathfindingMode _pathMode;
    CornersDetector _cornerMode;
    MasterNavigation _navigation;
    Piezo _piezo;
};

#endif
