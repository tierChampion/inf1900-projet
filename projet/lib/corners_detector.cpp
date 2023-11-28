#include "corners_detector.h"

CornersDetector::CornersDetector(MasterNavigation *navigation)
    : _detector(0),
      _isDetecting(false),
      _scan(0),
      _intersection(LineStructure::NONE),
      _navigation(navigation),
      _lineSensor(navigation->getLineSensor())

{
}

const char *CornersDetector::run()
{
    findCorner();
    comeBack();
    return detect();
}

void CornersDetector::findCorner()
{
    EventTimer::setToggling(true);
    _scan = 0;
    _detector = 0;
    _isDetecting = true;
    EventTimer::resetNavigationCounter();
    _navigation->jumpStart();
    while (_isDetecting)
    {
        _navigation->goStraight();
        _lineSensor->updateDetection();
        if (_lineSensor->getStructure() == LineStructure::RIGHT || _lineSensor->getStructure() == LineStructure::LEFT)
        {
            _intersection = _lineSensor->getStructure();
            while (_lineSensor->detectsSimpleIntersection())
            {
                _lineSensor->updateDetection();
            }

            // lineSensor.updateDetection();
            scan();
            // center
            _navigation->driveDistance(42);
        }
        PRINT(_detector);
    }
    PRINT("FIRST UTURN");
}

void CornersDetector::comeBack()
{
    if (_intersection == LineStructure::RIGHT)
    {
        _navigation->executeMovementCode(MovementCode::LEFT);
    }
    else
    {
        _navigation->executeMovementCode(MovementCode::RIGHT);
    }
    if (_detector == 0b010011 || _detector == 0b011011)
        _navigation->driveToIntersection();
    _navigation->driveToIntersection();
    PRINT("SECOND UTURN");
    if (_intersection == LineStructure::RIGHT)
    {
        _navigation->executeMovementCode(MovementCode::RIGHT);
    }
    else
    {
        _navigation->executeMovementCode(MovementCode::LEFT);
    }
}
void CornersDetector::scan()
{

    _isDetecting = false;
    _detector |= (_detector == 0 &&
                  EventTimer::getNavigationCounter() > (MasterNavigation::ONE_UNIT_COUNT + (MasterNavigation::ONE_UNIT_COUNT >> 2)))
                     ? (0b11 << 6)
                     : 0;

    PRINT("TIME TAKEN");
    PRINT(EventTimer::getNavigationCounter());

    LineStructure detection = _lineSensor->getStructure() == LineStructure::NONE ? _intersection : _lineSensor->getStructure();

    switch (detection)
    {
    case LineStructure::RIGHT:
        _detector |= 0b010 << _scan;
        break;
    case LineStructure::RIGHT_FORWARD:
        _detector |= 0b011 << _scan;
        _isDetecting = true;
        _scan += 3;
        if (_detector == 0b11000011)
        {
            _isDetecting = false;
        }
        break;
    case LineStructure::LEFT:
        _detector |= 0b100 << _scan;
        break;
    case LineStructure::LEFT_FORWARD:
        _detector |= 0b101 << _scan;

        break;
    default:
        break;
    }
    if (_scan == 6)
    {
        _isDetecting = false;
    }
    PRINT(structureToString(detection));
}

const char *CornersDetector::detect()
{
    const char *corner = "CORNER NOT DETECTED";
    switch (_detector) // 3 derniers bits pour encodage de la 1er detection
    {                  // 3 bits du milieu pour la 2eme

    case 0b11000011: // RIGHT_FORWARD Long
        return "(1,1) EAST";
        break;
    case 0b100: // LEFT
        corner = "(1,1) SOUTH";
        break;
    case 0b11000100: // LEFT long
        corner = "(1,4) EAST";
        break;
    case 0b010: // RIGHT
        corner = "(1,4) NORTH";
        break;
    case 0b011011: // RIGHT_FORWARD  and RIGHT_FORWARD
        corner = "(4,7) WEST";
        break;
    case 0b11000101: // LEFT_FORWARD long
        corner = "(4,7) NORTH";
        break;
    case 0b010011: // RIGHT_FORWARD and RIGHT
        return "(1,7) SOUTH";
        break;
    case 0b101: // LEFT_FORWARD
        return "(1,7) WEST";
        break;
    default:
        break;
    }

    PRINT(corner);
    return corner;
}