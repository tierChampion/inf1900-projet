#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "wheel.h"

class Navigation
{
public:
    Navigation();
    ~Navigation();
    void pivot90(Side turn);
    void pivot(Side turn);
    void moveStraight(Orientation orientation, uint8_t speed);
    void moveStraight(Orientation orientation);
    void realForward();
    void adjustWheel(Side turn, uint8_t intensity);
    void stop();
    void jumpStart();
    void turnJumpStart(Side turn);

private:
    Timer0 _timerPWM;
    Wheel _leftWheel;
    Wheel _rightWheel;
    WritePin _leftEnablePin;
    WritePin _rightEnablePin;
};

#endif
