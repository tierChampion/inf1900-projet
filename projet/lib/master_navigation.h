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

    void drive();
    // turn left
    void pivot(Side turn);
    void turn(Side turn);
    // turn right and mesure the angle (mesured)
    void turnMesuredRight();

    void stop();

    void executeMovementCodes(MovementCode *codes, uint8_t length);

private:
    void driveDistance(uint16_t distance);
    void goStraight();

private:
    Navigation _navigation;
    LineSensor _lineSensor;
    DistanceSensor _distSensor;
    // Timer1 _mesureTimer;
};

#endif