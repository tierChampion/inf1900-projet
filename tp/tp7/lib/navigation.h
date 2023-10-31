#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "wheel.h"

class Navigation
{
public:
    Navigation(WritePin dirLeftPin, WritePin dirRightPin);
    void turn(Direction direction, double speed);
    void move(Direction direction, double speed);
    void stop();

private:
    Wheel _leftWheel;
    Wheel _rightWheel;
    Timer0 _timerPWM;
};

#endif
