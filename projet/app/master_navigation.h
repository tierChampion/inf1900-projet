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

    uint16_t getUnitCount() const;

    // forward until intersection
    void driveToIntersection(bool calibrate = false);

    // forward for one unit (mesured)
    void driveOneUnit();

    void drive();
    // turn left
    void pivot(Side turn, bool isTurning = false);
    void turn(Side turn);
    // turn right and mesure the angle (mesured)
    void uTurn();
    void jumpStart();
    void stop();

    void executeMovementCode(MovementCode code, bool calibrate = false);

    // en mode public pour les tests de detection de corners
    void driveDistance(uint16_t distance);
    void goStraight();

    LineSensor *getLineSensor()
    {
        return &_lineSensor;
    }

private:
    void calibrateDistances(uint16_t distCount);

private:
    Navigation _navigation;
    LineSensor _lineSensor;
    uint16_t _centeringCount;
    uint16_t _unitCount;
};

#endif