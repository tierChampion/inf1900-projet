#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "wheel.h"

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
