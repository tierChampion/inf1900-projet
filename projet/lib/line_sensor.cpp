#include "line_sensor.h"

LineSensor::LineSensor() : _digitalInputLeft(ReadPin(Port::A, PA1)),
                           _digitalInputMiddleLeft(ReadPin(Port::A, PA2)),
                           _digitalInputMiddle(ReadPin(Port::A, PA3)),
                           _digitalInputMiddleRight(ReadPin(Port::A, PA4)),
                           _digitalInputRight(ReadPin(Port::A, PA5)),
                           _structure(LineStructure::NONE),
                           _sensorArray(0)
{
}

void LineSensor::updateDetection()
{
    _sensorArray = readSensorArray();

    if (((_sensorArray & 0b10001) == 0b10001))
    {

        _structure = LineStructure::T;
    }
    else if (((_sensorArray & 0b00011) == 0b00011))
    {
        _structure = LineStructure::LEFT;
    }
    else if (((_sensorArray & 0b11000) == 0b11000))
    {
        _structure = LineStructure::RIGHT;
    }
    else if (((_sensorArray & 0b00100) == 0b00100) || ((_sensorArray & 0b01000) == 0b01000) || ((_sensorArray & 0b00010) == 0b00010))
    {
        switch (_structure)
        {
        case LineStructure::T:
            _structure = LineStructure::CROSS;
            break;
        case LineStructure::RIGHT:
            _structure = LineStructure::RIGHT_FORWARD;
            break;
        case LineStructure::LEFT:
            _structure = LineStructure::LEFT_FORWARD;
            break;
        default:
            _structure = LineStructure::FORWARD;
            break;
        }
    }
    else
    {
        _structure = LineStructure::NONE;
    }
}

bool LineSensor::needLeftAdjustment() const
{
    return ((_sensorArray & 0b00010) != 0) &&
           ((_sensorArray & 0b11000) == 0) &&
           ((_sensorArray & 0b00101) != 0b00101);
}

bool LineSensor::needRightAdjustment() const
{
    return ((_sensorArray & 0b01000) != 0) &&
           ((_sensorArray & 0b00011) == 0) &&
           ((_sensorArray & 0b10100) != 0b10100);
}

bool LineSensor::detectsIntersection() const
{
    return _structure != LineStructure::FORWARD;
}

uint8_t LineSensor::readSensorArray() const
{
    return (_digitalInputLeft.read() |
            _digitalInputMiddleLeft.read() |
            _digitalInputMiddle.read() |
            _digitalInputMiddleRight.read() |
            _digitalInputRight.read()) >>
           1;
}
uint8_t LineSensor::readSensor(uint8_t index) const
{
    switch (index)
    {
    case 1:
        return _digitalInputLeft.read();
    case 2:
        return _digitalInputMiddleLeft.read();

    case 3:
        return _digitalInputMiddle.read();

    case 4:
        return _digitalInputMiddleRight.read();

    case 5:
        return _digitalInputRight.read();

    default:
        PRINT("ERROR");
        break;
    }
    return -1;
}
LineStructure LineSensor::getStructure() const { return _structure; }
