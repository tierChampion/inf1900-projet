#include "corners_detector.h"
const uint8_t ONE_UNIT_DISTANCE = 140;
CornersDetector::CornersDetector()
{
    _isDetecting = false;
    _scanState = ScanState::FIRST;
}

const char *CornersDetector::detectCorner(MasterNavigation navigation, LineSensor lineSensor)
{
    uint16_t distanceTotal = 0;
    _scanState = ScanState::FIRST;
    _isDetecting = true;
    EventTimer::resetNavigationCounter();
    while (_isDetecting)
    {
        navigation.goStraight();
        lineSensor.updateDetection();
        if (lineSensor.detectsIntersection())
        {
            navigation.drive();
            _delay_ms(400);
            lineSensor.updateDetection();
            scan(lineSensor);
        }
    }
    PRINT("");
    //  distanceTotal = EventTimer::getNavigationCounter();
    // navigation.uTurn();
    //  navigation.driveDistance(distanceTotal);
    // navigation.uTurn();
    return detect();
}
void CornersDetector::scan(LineSensor lineSensor)
{

    uint8_t tempScan = (uint8_t)_scanState;
    _isDetecting = false;

    switch (lineSensor.getStructure())
    {
    case LineStructure::RIGHT:
        _detector |= 0b010 << tempScan;
        break;
    case LineStructure::RIGHT_FORWARD:
        _detector |= (!_detector && EventTimer::getNavigationCounter() > ONE_UNIT_DISTANCE) ? (0b11 << 6) : 0;
        _detector |= 0b011 << tempScan;
        _isDetecting = true;
        tempScan += 3;
        break;
    case LineStructure::LEFT:
        _detector |= (!_detector && EventTimer::getNavigationCounter() > ONE_UNIT_DISTANCE) ? (0b11 << 6) : 0;
        _detector |= 0b100 << tempScan;

        break;
    case LineStructure::LEFT_FORWARD:
        _detector |= (!_detector && EventTimer::getNavigationCounter() > ONE_UNIT_DISTANCE) ? (0b11 << 6) : 0;

        _detector |= 0b101 << tempScan;

        break;
    default:
        break;
    }
    _scanState = (ScanState)tempScan;
    if (_scanState == ScanState::OVER)
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
        corner = "(1,1) EAST";
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
        corner = "(1,7) SOUTH";
        break;
    case 0b101: // LEFT_FORWARD
        corner = "(1,7) WEST";
        break;
    default:
        break;
    }
    PRINT(corner);
    return corner;
}