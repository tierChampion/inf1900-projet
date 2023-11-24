#include "corners_detector.h"
const uint8_t ONE_UNIT_DISTANCE = 150;
CornersDetector::CornersDetector()
{
    _detector = 0;
    _isDetecting = false;
    _scan = 0;
}

const char *CornersDetector::detectCorner(MasterNavigation navigation, LineSensor lineSensor)
{
    _scan = 0;
    _detector = 0;
    _isDetecting = true;
    EventTimer::resetNavigationCounter();
    navigation.jumpStart();

    while (_isDetecting)
    {
        navigation.goStraight();
        lineSensor.updateDetection();
        if (lineSensor.getStructure() == LineStructure::RIGHT || lineSensor.getStructure() == LineStructure::LEFT)
        {
            while (lineSensor.notIntersection())
            {
            }
            lineSensor.updateDetection();
            scan(lineSensor);
            navigation.driveDistance(42);
        }
        PRINT(_detector);
    }
    PRINT("FIRST UTURN");
    navigation.executeMovementCode(MovementCode::UTURN);
    if (_detector == 0b010011 || _detector == 0b011011)
        navigation.driveToIntersection();
    navigation.driveToIntersection();
    PRINT("SECOND UTURN");
    navigation.executeMovementCode(MovementCode::UTURN);

    return detect();
}
void CornersDetector::scan(LineSensor lineSensor)
{

    _isDetecting = false;
    _detector |= (_detector == 0 && EventTimer::getNavigationCounter() > ONE_UNIT_DISTANCE) ? (0b11 << 6) : 0;

    switch (lineSensor.getStructure())
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
    PRINT(structureToString(lineSensor.getStructure()));
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