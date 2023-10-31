#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "wheel.h"

class Navigation
{
public:
    Navigation(WritePin dirLeftPin, WritePin dirRightPin);
    ~Navigation();
    void controlledTurn(Side turn, Orientation orientation, float speed, uint8_t turnStrength);
    void moveStraight(Orientation orientation, float speed);
    void stop();

private:
    Wheel _leftWheel;
    Wheel _rightWheel;
    Timer0 _timerPWM;
};

#endif
