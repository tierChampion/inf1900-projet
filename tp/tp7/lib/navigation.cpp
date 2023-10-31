#include "navigation.h"

Navigation::Navigation(WritePin dirLeftPin, WritePin dirRightPin)
{
    Timer0 timerPWM{};
    timerPWM.setCounterValue(0);
    timerPWM.setPrescalar(TimerPrescalar::NO_PRESCALAR);
    timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::CLEAR);
    timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::CLEAR);
    timerPWM.setWaveMode(TimerWaveMode::PWM_PHASE_CORRECT);
    _leftWheel = Wheel(dirLeftPin, Side::LEFT, &timerPWM);
    _rightWheel = Wheel(dirRightPin, Side::RIGHT, &timerPWM);
}

void Navigation::turn(Direction direction, double speed)
{
    switch (direction)
    {
    case Direction::LEFT:
        _leftWheel.setSpeed(Direction::FORWARD, 0);
        _rightWheel.setSpeed(Direction::FORWARD, speed);
        break;
    case Direction::RIGHT:
        _leftWheel.setSpeed(Direction::FORWARD, speed);
        _rightWheel.setSpeed(Direction::FORWARD, 0);
        break;
    default:
        break;
    }
}

void Navigation::move(Direction direction, double speed)
{
    _leftWheel.setSpeed(direction, speed);
    _rightWheel.setSpeed(direction, speed);
}

void Navigation::stop()
{
    move(Direction::FORWARD, 0);
}
