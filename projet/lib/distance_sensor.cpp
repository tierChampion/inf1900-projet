#include "distance_sensor.h"

const uint8_t CLOSE_DISTANCE = 200; // TO MODIFY WITH RIGHT CALIBRATION
const uint8_t CAN_SHIFT = 2;

DistanceSensor::DistanceSensor() : _converter(can()), _inputPin(ReadPin(Port::A, PA0))
{
}

bool DistanceSensor::isClose()
{
    uint8_t dist = getDistance(); 
    PRINT(dist);
    return dist < CLOSE_DISTANCE;
}

uint8_t DistanceSensor::getDistance()
{

    uint16_t rawDistance = _converter.lecture(_inputPin.getPosition());

    return MAXIMUM_8BIT - static_cast<uint8_t>(rawDistance >> CAN_SHIFT);
}