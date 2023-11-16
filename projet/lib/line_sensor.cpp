#include "line_sensor.h"

LineSensor::LineSensor() : _digitalInputLeft(ReadPin(Port::A, PA1)),
                           _digitalInputMiddleLeft(ReadPin(Port::A, PA2)),
                           _digitalInputMiddle(ReadPin(Port::A, PA3)),
                           _digitalInputMiddleRight(ReadPin(Port::A, PA4)),
                           _digitalInputRight(ReadPin(Port::A, PA5)),
                           _structure(LineStructure::NONE)
{
}

void LineSensor::updateDetection()
{
    uint8_t reading = readSensorArray();

    if (((reading & 0b10001) == 0b10001))
    {

        _structure = LineStructure::T;
    }
    else if (((reading & 0b00011) == 0b00011))
    {
        _structure = LineStructure::LEFT;
    }
    else if (((reading & 0b11000) == 0b11000))
    {
        _structure = LineStructure::RIGHT;
    }
    else if (((reading & 0b00100) == 0b00100))
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
LineStructure LineSensor::getStructure() { return _structure; }
