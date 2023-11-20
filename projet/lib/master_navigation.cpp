#include "master_navigation.h"

MasterNavigation::MasterNavigation() : _navigation(Navigation()),
                                       _lineSensor(LineSensor()),
                                       _distSensor(DistanceSensor()),
                                       _mesureTimer(Timer1())
{
    // setting of the timer1

    // what mode?
    // what prescalar?
    // which compare for the measures? (leave 1 for the flashing led)
}

void MasterNavigation::driveToIntersection()
{
    // drive forward while adjusting.

    // check for intersections.

    // if intersection, center on it and stop.
}

void MasterNavigation::driveOneUnit()
{
    // launch the measure timer for the needed time (to determine)

    // drive forward while adjusting

    // if time is done, stop

    // stop the timer
}

void MasterNavigation::turnLeft()
{
    // pivot left

    // check for a new line

    // if centered on the new line stop
}

void MasterNavigation::turnRight()
{
    // pivot right

    // check for a new line

    // if centered on the new line stop
}

void MasterNavigation::turnMesuredRight()
{

    // launch the measure timer for the needed time (to determine)

    // pivot right

    // check for a new line

    // if centered on the new line stop

    // stop the timer and check the time measured

    // determine if the angle is about 180
}
