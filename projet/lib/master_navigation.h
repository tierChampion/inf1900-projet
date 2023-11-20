#ifndef MASTER_NAVIGATION_H
#define MASTER_NAVIGATION_H

#include "navigation.h"
#include "line_sensor.h"
#include "timer1.h"
#include "distance_sensor.h"

class MasterNavigation
{
public:
    MasterNavigation();

    // forward until intersection
    void driveToIntersection();
    // forward for one unit (mesured)
    void driveOneUnit();
    // turn left
    void turnLeft();
    // turn right
    void turnRight();
    // turn right and mesure the angle (mesured)
    void turnMesuredRight();

private:
    Navigation _navigation;
    LineSensor _lineSensor;
    DistanceSensor _distSensor;
    Timer1 _mesureTimer;
};

#endif