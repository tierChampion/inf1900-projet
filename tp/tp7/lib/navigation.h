#include "wheel.h"
#ifndef NAVIGATION_H
#define NAVIGATION_H
/*  PB3-PB4-TIMER0 occupied | speed is from 0 to 100(%) */
class Navigation
{
public:
    Navigation(Pin dirRightPin, Pin dirLeftPin);
    ~Navigation();
    void turn(Direction direction, uint8_t speed);
    void move(Direction direction, uint8_t speed);
    void stop();
    void setDirPin(Side side, Pin directionPin);
    Pin getDirPin(Side side) const;

private:
    Wheel _rightWheel;
    Wheel _leftWheel;
};
#endif
