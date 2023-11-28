#include "master_navigation.h"
#include "event_timer.h"

const uint8_t STABILIZING_DELAY = 250;

const uint8_t INTERSECTION_CENTERING_COUNT = 42;
const uint8_t UTURN_COUNT = 90;

// robot 1
const uint8_t LEFT_ADJUST_STRENGTH = 10;
const uint8_t RIGHT_ADJUST_STRENGTH = 30;

MasterNavigation::MasterNavigation() : _navigation(Navigation()),
                                       _lineSensor(LineSensor())
{
}

uint16_t MasterNavigation::driveToIntersection()
{
    // drive forward while adjusting.
    bool running = true;

    uint16_t calibration = 0;

    _navigation.jumpStart();
    _navigation.moveStraight(Orientation::FORWARD);

    EventTimer::resetNavigationCounter();

    while (running)
    {
        goStraight();
        // check for intersections.
        if (_lineSensor.detectsIntersection() && EventTimer::getNavigationCounter() >= 35) // 3/4 du centrage
        {
            calibration = EventTimer::getNavigationCounter();
            _navigation.realForward();

            while (_lineSensor.detectsSimpleIntersection())
            {
                _lineSensor.updateDetection();
            }

            driveDistance(INTERSECTION_CENTERING_COUNT);

            running = false;
        }
    }

    return calibration;
}

void MasterNavigation::driveOneUnit()
{
    driveDistance(MasterNavigation::ONE_UNIT_COUNT);
}

void MasterNavigation::driveDistance(uint16_t distance)
{
    _navigation.jumpStart();
    _navigation.moveStraight(Orientation::FORWARD);

    EventTimer::resetNavigationCounter();

    while (EventTimer::getNavigationCounter() <= distance)
    {
        goStraight();
    }

    _navigation.stop();
}

void MasterNavigation::jumpStart()
{
    _navigation.jumpStart();
}

void MasterNavigation::goStraight()
{
    _lineSensor.updateDetection();

    if (_lineSensor.getStructure() == LineStructure::FORWARD)
    {
        if (_lineSensor.needLeftAdjustment())
        {
            _navigation.adjustWheel(Side::LEFT, LEFT_ADJUST_STRENGTH);
        }
        else if (_lineSensor.needRightAdjustment())
        {
            _navigation.adjustWheel(Side::RIGHT, RIGHT_ADJUST_STRENGTH);
        }
        else
        {
            _navigation.moveStraight(Orientation::FORWARD);
        }
    }
}

void MasterNavigation::drive()
{
    _navigation.jumpStart();
    _navigation.realForward();
}

void MasterNavigation::pivot(Side turn)
{
    _navigation.turnJumpStart(turn);
    _navigation.pivot(turn);

    uint8_t lineDetected = 0;

    while (lineDetected < 2)
    {
        _lineSensor.updateDetection();

        if (_lineSensor.getStructure() == LineStructure::NONE && lineDetected == 0)
        {
            lineDetected = 1;
        }

        if (_lineSensor.getStructure() == LineStructure::FORWARD && lineDetected == 1)
        {
            _navigation.stop();
            lineDetected = 2;
        }
    }
}

void MasterNavigation::turn(Side turn)
{
    _navigation.turnJumpStart(turn);
    _navigation.pivot(turn);
}

void MasterNavigation::uTurn()
{
    EventTimer::resetNavigationCounter();
    turn(Side::LEFT);

    while (EventTimer::getNavigationCounter() <= UTURN_COUNT)
    {}

    pivot(Side::LEFT);
}

void MasterNavigation::stop()
{
    _navigation.stop();
}

void MasterNavigation::executeMovementCode(MovementCode code)
{
    switch (code)
    {
    case MovementCode::FORWARD:
        driveToIntersection();
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::FORWARD_1:
        driveOneUnit(); // forward one unit
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::LEFT:
        pivot(Side::LEFT);
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::RIGHT:
        pivot(Side::RIGHT);
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::LEFT_FORWARD:
        pivot(Side::LEFT);
        _delay_ms(STABILIZING_DELAY);
        driveToIntersection();
        break;

    case MovementCode::RIGHT_FORWARD:
        pivot(Side::RIGHT);
        _delay_ms(STABILIZING_DELAY);
        driveToIntersection();
        break;

    case MovementCode::UTURN:
        uTurn();
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::UTURN_FORWARD:
        uTurn();
        _delay_ms(STABILIZING_DELAY);
        driveToIntersection();
        break;

    default:
        break;
    }
}
