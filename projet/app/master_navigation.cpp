#include "master_navigation.h"
#include "event_timer.h"

/**
 * /fichier master_navigation.cpp
 * /auteurs Thierry Champion, Nikolai Olekhnovitch, Raisse Oumarou Petitot, Rym Touati
 * /date    05 décembre 2023
 * Créé le 10 novembre
 * 
 * Classe représentant la navigation avancé avec les capteurs et les mesures. La MasterNavigation 
 * est capable de plusieurs movements ajustées et mesurés. Avec ses mesures, la navigation est 
 * aussi capable de se calibrer automatiquement en estimant les distances.
 */

const uint8_t STABILIZING_DELAY = 250;

const uint16_t DEFAULT_ONE_UNIT_COUNT = 134;
const uint8_t DEFAULT_INTERSECTION_CENTERING_COUNT = 44;
const uint8_t UTURN_COUNT = 90;
const uint8_t ESTIMATED_LINECROSSING_COUNT = 5;

const uint8_t LEFT_ADJUST_STRENGTH = 10;
const uint8_t RIGHT_ADJUST_STRENGTH = 30;

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
    bool running = true;

    _navigation.jumpStart();
    _navigation.moveStraight(Orientation::FORWARD);

    EventTimer::resetNavigationCounter();

    while (running)
    {
        goStraight();
        
        if (_lineSensor.detectsIntersection() &&
            EventTimer::getNavigationCounter() >= ((_centeringCount >> 1) + (_centeringCount >> 2)))
        {
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

    uint8_t lineDetected = INIT;

    while (lineDetected != LINE_DETECTED)
    {
        _lineSensor.updateDetection();

        if (_lineSensor.getStructure() == LineStructure::NONE && lineDetected == INIT)
        {
            lineDetected = NO_LINE_DETECTED;
        }

        if (_lineSensor.getStructure() == LineStructure::FORWARD && lineDetected == NO_LINE_DETECTED)
        {
            _navigation.stop();
            lineDetected = LINE_DETECTED;
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

    case MovementCode::UTURN:
        uTurn();
        _delay_ms(STABILIZING_DELAY);
        break;

    default:
        break;
    }
}
