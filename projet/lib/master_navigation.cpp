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
    bool running = true;

    _navigation.jumpStart();
    _navigation.moveStraight(Orientation::FORWARD);

    while (running)
    {
        _lineSensor.updateDetection();

        if (_lineSensor.getStructure() == LineStructure::FORWARD)
        {
            if (_lineSensor.needLeftAdjustment())
            {
                _navigation.adjustWheel(Side::LEFT, 10);
            }
            else if (_lineSensor.needRightAdjustment())
            {
                _navigation.adjustWheel(Side::RIGHT, 30);
            }
            else
            {
                _navigation.moveStraight(Orientation::FORWARD);
            }
        }
        // check for intersections.
        else if (_lineSensor.detectsIntersection())
        {
            // if intersection, center on it and stop.
            _navigation.realForward();
            _delay_ms(500); // crosses the intersection but doesnt center
            _lineSensor.updateDetection();
            PRINT(_lineSensor.detectsIntersection() ? "INT" : "NOPE");
            _navigation.stop();
            running = false;
        }
    }
}

void MasterNavigation::driveOneUnit()
{
    // launch the measure timer for the needed time (to determine)

    // drive forward while adjusting

    // if time is done, stop

    // stop the timer
}

void MasterNavigation::pivot(Side turn)
{
    // pivot left
    _navigation.pivot(turn);

    _delay_ms(500);

    bool running = true;
    // check for a new line
    while (running)
    {
        _lineSensor.updateDetection();

        // mix with t?
        if (_lineSensor.getStructure() == LineStructure::FORWARD)
        {
            _navigation.stop();
            running = false;
        }
    }

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

void MasterNavigation::executeMovementCodes(MovementCode *codes, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        switch (codes[i])
        {
        case MovementCode::FORWARD:
            driveToIntersection();
            break;
        case MovementCode::LEFT:
            pivot(Side::LEFT);
            break;
        case MovementCode::RIGHT:
            pivot(Side::RIGHT);
            break;
        case MovementCode::LEFT_FORWARD:
            pivot(Side::LEFT);
            _delay_ms(100);
            driveToIntersection();
            break;
        case MovementCode::RIGHT_FORWARD:
            pivot(Side::RIGHT);
            _delay_ms(100);
            driveToIntersection();

        default:
            break;
        }
    }
}
