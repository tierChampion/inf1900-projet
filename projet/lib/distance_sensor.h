#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include "pins.h"
#include "can.h"

class DistanceSensor
{

public:
    DistanceSensor();
    bool isClose();

private:
    uint8_t getDistance();

private:
    can _converter;
    const ReadPin _inputPin;
};

#endif