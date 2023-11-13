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
    uint8_t reading = (_digitalInputLeft.read() |
                       _digitalInputMiddleLeft.read() |
                       _digitalInputMiddle.read() |
                       _digitalInputMiddleRight.read() |
                       _digitalInputRight.read());

    if (((reading & 0b100010) == 0b100010))
    {
        _structure = LineStructure::T;
    }
    else if (((reading & 0b000110) == 0b000110))
    {
        _structure = LineStructure::LEFT;
    }
    else if (((reading & 0b110000) == 0b110000))
    {
        _structure = LineStructure::RIGHT;
    }
    else if (((reading & 0b001000) == 0b001000))
    {
        _structure = LineStructure::FORWARD;
    }
    else
    {
        _structure = LineStructure::NONE;
    }
}

LineStructure LineSensor::getStructure() { return _structure; }
