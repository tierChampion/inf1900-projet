#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include "pins.h"

enum class LineStructure
{
    NONE,
    FORWARD,
    LEFT,
    RIGHT,
    T,
    CROSS,
    RIGHT_FORWARD,
    LEFT_FORWARD

};

inline const char *structureToString(LineStructure structure)
{
    switch (structure)
    {
    case LineStructure::NONE:
        return "No line";
    case LineStructure::FORWARD:
        return "Straight line";
    case LineStructure::LEFT:
        return "Left turn";
    case LineStructure::RIGHT:
        return "Right turn";
    case LineStructure::T:
        return "T intersection";
    default:
        return "UNKNOWN!";
    }
}

class LineSensor
{

public:
    LineSensor();
    void updateDetection();
    LineStructure getStructure();
    uint8_t readSensor(uint8_t index) const;
    uint8_t readSensorArray() const;

private:
    ReadPin _digitalInputLeft;
    ReadPin _digitalInputMiddleLeft;
    ReadPin _digitalInputMiddle;
    ReadPin _digitalInputMiddleRight;
    ReadPin _digitalInputRight;
    LineStructure _structure;
};

#endif