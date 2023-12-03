#include "line_sensor.h"
const uint8_t MASK_FIRST_DIGITAL_INPUT = 0b00001;
const uint8_t MASK_FOURTH_DIGITAL_INPUT = 0b01000;
const uint8_t MASK_THIRD_DIGITAL_INPUT = 0b00100;
const uint8_t MASK_SECOND_DIGITAL_INPUT = 0b00010;
const uint8_t MASK_FIFTH_DIGITAL_INPUT = 0b10000;

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

    if (((_sensorArray & (MASK_FIRST_DIGITAL_INPUT | MASK_FIFTH_DIGITAL_INPUT)) == (MASK_FIRST_DIGITAL_INPUT | MASK_FIFTH_DIGITAL_INPUT)))
    {
        _structure = LineStructure::T;
    }
    else if (((_sensorArray & (MASK_FIRST_DIGITAL_INPUT | MASK_SECOND_DIGITAL_INPUT)) == (MASK_FIRST_DIGITAL_INPUT | MASK_SECOND_DIGITAL_INPUT)))
    {
        _structure = LineStructure::LEFT;
    }
    else if (((_sensorArray & (MASK_FOURTH_DIGITAL_INPUT | MASK_FIFTH_DIGITAL_INPUT)) == (MASK_FOURTH_DIGITAL_INPUT | MASK_FIFTH_DIGITAL_INPUT)))
    {
        _structure = LineStructure::RIGHT;
    }
    else if (((_sensorArray & MASK_THIRD_DIGITAL_INPUT) == MASK_THIRD_DIGITAL_INPUT) || ((_sensorArray & MASK_FOURTH_DIGITAL_INPUT) == MASK_FOURTH_DIGITAL_INPUT) || ((_sensorArray & MASK_SECOND_DIGITAL_INPUT) == MASK_SECOND_DIGITAL_INPUT))
    {
        _structure = LineStructure::FORWARD;
    }
    else
    {
        _structure = LineStructure::NONE;
    }
}

bool LineSensor::needLeftAdjustment() const
{
    return ((_sensorArray & MASK_SECOND_DIGITAL_INPUT) != 0) &&
           ((_sensorArray & (MASK_FOURTH_DIGITAL_INPUT | MASK_FIFTH_DIGITAL_INPUT)) == 0) &&
           ((_sensorArray & (MASK_FIRST_DIGITAL_INPUT | MASK_THIRD_DIGITAL_INPUT)) != (MASK_FIRST_DIGITAL_INPUT | MASK_THIRD_DIGITAL_INPUT));
}

bool LineSensor::needRightAdjustment() const
{
    return ((_sensorArray & MASK_FOURTH_DIGITAL_INPUT) != 0) &&
           ((_sensorArray & (MASK_FIRST_DIGITAL_INPUT | MASK_SECOND_DIGITAL_INPUT)) == 0) &&
           ((_sensorArray & (MASK_FIFTH_DIGITAL_INPUT | MASK_THIRD_DIGITAL_INPUT)) != (MASK_FIFTH_DIGITAL_INPUT | MASK_THIRD_DIGITAL_INPUT));
}

bool LineSensor::detectsIntersection() const
{
    return _structure != LineStructure::FORWARD; // to check
}

bool LineSensor::detectsSimpleIntersection() const
{
    return _structure == LineStructure::LEFT || _structure == LineStructure::RIGHT || _structure == LineStructure::T;
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
