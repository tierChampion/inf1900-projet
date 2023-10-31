#ifndef WHEEL_H
#define WHEEL_H

#include <avr/interrupt.h>

#include "pins.h"
#include "timer0.h"

enum class Side
{
    RIGHT,
    LEFT
};

enum class Direction
{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};

class Wheel
{

public:
    Wheel();
    Wheel(WritePin directionPin, Side side, Timer0 *timerPwm);
    ~Wheel();
    void setSpeed(Direction direction, double speed);

private:
    WritePin _directionPin;
    Side _side;
    Timer0 *_timerPWM;
};

#endif
