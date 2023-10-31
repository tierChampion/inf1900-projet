#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "wheel.h"

class Navigation
{
public:
    Navigation(WritePin dirLeftPin, WritePin dirRightPin);
    void controledTurn(Side turn, Orientation orientation, float speed, uint8_t turnStrength);
    void move(Direction direction, float speed);
    void stop();

private:
    Wheel _leftWheel;
    Wheel _rightWheel;
    Timer0 _timerPWM;
};

#endif
