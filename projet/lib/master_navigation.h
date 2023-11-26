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
    void uTurn();
    void jumpStart();
    void stop();

    void executeMovementCode(MovementCode code);

    // en mode public pour les tests de detection de corners
    void driveDistance(uint16_t distance);
    void goStraight();

public:
    static const uint16_t ONE_UNIT_COUNT = 130;

    // tc / t = 14 / 26 ~ 1/2

private:
    Navigation _navigation;
    LineSensor _lineSensor;
    DistanceSensor _distSensor;
    // Timer1 _mesureTimer;
};

#endif