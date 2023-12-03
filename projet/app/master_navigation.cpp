#include "master_navigation.h"
#include "event_timer.h"

const uint8_t STABILIZING_DELAY = 250;

const uint16_t DEFAULT_ONE_UNIT_COUNT = 134;
const uint8_t DEFAULT_INTERSECTION_CENTERING_COUNT = 44;
const uint8_t UTURN_COUNT = 90;
const uint8_t ESTIMATED_LINECROSSING_COUNT = 5;

const uint8_t LEFT_ADJUST_STRENGTH = 10;  // 10 pour 16
const uint8_t RIGHT_ADJUST_STRENGTH = 30; // 30 pour 16

MasterNavigation::MasterNavigation() : _navigation(Navigation()),
                                       _lineSensor(LineSensor()),
                                       _centeringCount(DEFAULT_INTERSECTION_CENTERING_COUNT),
                                       _unitCount(DEFAULT_ONE_UNIT_COUNT)
{
}

uint16_t MasterNavigation::getUnitCount() const
{
    return _unitCount;
}

void MasterNavigation::driveToIntersection(bool calibrate)
{
    // drive forward while adjusting.
    bool running = true;

    _navigation.jumpStart();
    _navigation.moveStraight(Orientation::FORWARD);

    EventTimer::resetNavigationCounter();

    while (running)
    {
        goStraight();
        // check for intersections.
        if (_lineSensor.detectsIntersection() &&
            EventTimer::getNavigationCounter() >= ((_centeringCount >> 1) + (_centeringCount >> 2))) // 3/4 du centrage
        {
            // TO TEST!!! (see pathfindingMode travelPath as well)
            if (calibrate)
                calibrateDistances(EventTimer::getNavigationCounter());

            _navigation.realForward();

            while (_lineSensor.detectsSimpleIntersection())
            {
                _lineSensor.updateDetection();
            }

            driveDistance(_centeringCount);

            running = false;
        }
    }
}

void MasterNavigation::driveOneUnit()
{
    driveDistance(_unitCount);
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
    else
    {
        _navigation.moveStraight(Orientation::FORWARD);
    }
}

void MasterNavigation::drive()
{
    _navigation.jumpStart();
    _navigation.realForward();
}

void MasterNavigation::pivot(Side turn, bool isTurning)
{
    if (!isTurning)
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
    {
    }

    pivot(Side::LEFT, true);
}

void MasterNavigation::calibrateDistances(uint16_t distCount)
{
    _centeringCount = (distCount >> 1) + (distCount >> 2) - 2;
    _unitCount = distCount + _centeringCount + ESTIMATED_LINECROSSING_COUNT;
}

void MasterNavigation::stop()
{
    _navigation.stop();
}

void MasterNavigation::executeMovementCode(MovementCode code, bool calibrate)
{
    switch (code)
    {
    case MovementCode::FORWARD:
        driveToIntersection(calibrate);
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::FORWARD_1:
        driveOneUnit();
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
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::RIGHT_FORWARD:
        pivot(Side::RIGHT);
        _delay_ms(STABILIZING_DELAY);
        driveToIntersection();
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::UTURN:
        uTurn();
        _delay_ms(STABILIZING_DELAY);
        break;

    case MovementCode::UTURN_FORWARD:
        uTurn();
        _delay_ms(STABILIZING_DELAY);
        driveToIntersection();
        _delay_ms(STABILIZING_DELAY);
        break;

    default:
        break;
    }
}
