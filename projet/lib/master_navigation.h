#ifndef MASTER_NAVIGATION_H
#define MASTER_NAVIGATION_H

#include "navigation.h"
#include "line_sensor.h"
#include "timer1.h"
#include "distance_sensor.h"
#include "movement_code.h"

class MasterNavigation
{
public:
    MasterNavigation();

    // forward until intersection
    void driveToIntersection();
    // forward for one unit (mesured)
    void driveOneUnit();
    // turn left
    void pivot(Side turn);
    // turn right and mesure the angle (mesured)
    void turnMesuredRight();

    void executeMovementCodes(MovementCode* codes, uint8_t length);

    void stop();

private:
    Navigation _navigation;
    LineSensor _lineSensor;
    DistanceSensor _distSensor;
    Timer1 _mesureTimer;
};

#endif