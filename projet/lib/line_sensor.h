#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include "pins.h"

enum class LineStructure : uint8_t
{
    NONE = 0,
    FORWARD = 1,
    LEFT = 2,
    RIGHT = 3,
    T = 4,
    CROSS = 5,
    RIGHT_FORWARD = 6,
    LEFT_FORWARD = 7

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
    case LineStructure::LEFT_FORWARD:
        return "Left forward";
    case LineStructure::RIGHT_FORWARD:
        return "Right forward";
    default:
        return "UNKNOWN!";
    }
}

class LineSensor
{

public:
    LineSensor();
    void updateDetection();
    LineStructure getStructure() const;
    bool needLeftAdjustment() const;
    bool needRightAdjustment() const;
    bool detectsIntersection() const;
    uint8_t readSensor(uint8_t index) const;
    uint8_t readSensorArray() const;
    bool intersection();

private:
    ReadPin _digitalInputLeft;
    ReadPin _digitalInputMiddleLeft;
    ReadPin _digitalInputMiddle;
    ReadPin _digitalInputMiddleRight;
    ReadPin _digitalInputRight;
    LineStructure _structure;
    uint8_t _sensorArray;
};

#endif