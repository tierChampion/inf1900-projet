#ifndef WHEEL_H
#define WHEEL_H

#include "commons.h"
#include <avr/interrupt.h>
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
    Wheel(Pin directionPin, Side side);
    ~Wheel();
    void setSpeed(Direction direction, float speed);
    Pin getDirPin() const;
    void setDirPin(Pin directionPin);

private:
    struct Pin _directionPin;
    Side _side;
    Timer0 _timerPWM = Timer0(TimerControls());
};
#endif
