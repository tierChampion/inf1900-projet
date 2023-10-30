#include "wheel.h"

#ifndef NAVIGATION_H
#define NAVIGATION_H
/*  PB3-PB4-TIMER0 occupied | speed is from 0 to 100(%) */
class Navigation
{
public:
    Navigation(WritePin dirLeftPin, WritePin dirRightPin);
    ~Navigation();
    void turn(Direction direction, float speed);
    void move(Direction direction, float speed);
    void stop();
    void setDirPin(Side side, WritePin directionPin);
    WritePin getDirPin(Side side) const;

private:
    Wheel _leftWheel;
    Wheel _rightWheel;
};
#endif
