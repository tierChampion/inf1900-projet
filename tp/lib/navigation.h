#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "wheel.h"

class Navigation
{
public:
    Navigation();
    ~Navigation();
    void pivot90(Side turn);
    void moveStraight(Orientation orientation, float speed);
    void moveStraight(Orientation orientation);
    void stop();

private:
    float _speed;
    Wheel _leftWheel;
    Wheel _rightWheel;
    Timer0 _timerPWM;
};

#endif
