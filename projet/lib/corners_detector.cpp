#include "corners_detector.h"

CornersDetector::CornersDetector()
{
    _detector = 0;
    _isDetecting = false;
    _scanState = ScanState::FIRST;
    _distanceCounter = 0;
}

const char *CornersDetector::detectCorner(MasterNavigation navigation, LineSensor lineSensor)
{
    _detector = 0;
    _scanState = ScanState::FIRST;
    _isDetecting = true;

    while (_isDetecting)
    {
        navigation.driveToIntersection();
        lineSensor.updateDetection();
        navigation.drive();
        _delay_ms(30);
        lineSensor.updateDetection();
        scan(lineSensor);
    }

    // uint16_t distantTotal = _distanceCounter;
    // navigation.drive(); //
    // navigation.pivot90(Side::RIGHT);
    // navigation.turn(Side::RIGHT);
    // while (_distanceCounter > 0)
    // {
    //   navigation.move();
    //  _delay_ms(20); // unite de distance 20ms
    // _distanceCounter--;
    // }
    return detect();
}
void CornersDetector::scan(LineSensor lineSensor)
{

    uint8_t tempScan = (uint8_t)_scanState;
    _isDetecting = false;

    switch (lineSensor.getStructure())
    {
    // case LineStructure::NONE:
    //   _detector |= 0b001 << tempScan;
    //   break;
    case LineStructure::RIGHT:
        _detector |= 0b010 << tempScan;
        break;
    case LineStructure::RIGHT_FORWARD:
        _detector |= (!_detector && _distanceCounter > 140) ? (0b11 << 6) : 0;
        _detector |= 0b011 << tempScan;
        if (_distanceCounter >> 6 == 0)
        {
            _isDetecting = true;
            tempScan += 3;
        }
        break;
    case LineStructure::LEFT:
        _detector |= (!_detector && _distanceCounter > 140) ? (0b11 << 6) : 0;
        _detector |= 0b100 << tempScan;

        break;
    case LineStructure::LEFT_FORWARD:
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
    PRINT(lineSensor.getStructure());
}
const char *CornersDetector::detect()
{
    switch (_detector) // 3 derniers bits pour encodage de la 1er detection
    {                  // 3 bits du milieu pour la 2eme

    case 0b11000011: // RIGHT_FORWARD Long
        return "(1,1) EAST";
        break;
    case 0b100: // LEFT
        return "(1,1) SOUTH";
        break;
    case 0b11000100: // LEFT long
        return "(1,4) EAST";
        break;
    case 0b010: // RIGHT
        return "(1,4) NORTH";
        break;
    case 0b011011: // RIGHT_FORWARD  and RIGHT_FORWARD
        return "(4,7) WEST";
        break;
    case 0b11000101: // LEFT_FORWARD long
        return "(4,7) NORTH";
        break;
    case 0b010011: // RIGHT_FORWARD and RIGHT
        return "(1,7) SOUTH";
        break;
    case 0b101: // LEFT_FORWARD
        return "(1,7) WEST";
        break;
    default:
        PRINT(string);
        return "CORNER NOT DETECTED";
        break;
    }

    return "";
}

void CornersDetector::intRoutineTimer1()
{
    _distanceCounter++;
}