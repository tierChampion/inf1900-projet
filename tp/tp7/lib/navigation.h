#include "wheel.h"

class Navigation // PB3-PB4-TIMER0 occupied | speed unit is percentage from -100 to 100
{
public:
    Navigation(Pin dirRightPin, Pin dirLeftPin);
    void turn(Side direction, uint8_t speed);
    void go(int8_t speed);
    void stop();
    void setDirPin(Side side, Pin directionPin);

private:
    Wheel _rightWheel;
    Wheel _leftWheel;
};
