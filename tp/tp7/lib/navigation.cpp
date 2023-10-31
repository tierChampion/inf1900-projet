#include "navigation.h"

Navigation::Navigation(WritePin dirLeftPin, WritePin dirRightPin)
{
    _timerPWM = Timer0();
    _timerPWM.setCounterValue(0);
    _timerPWM.setPrescalar(TimerPrescalar::NO_PRESCALAR);
    _timerPWM.setCompareMode(TimerCompare::A, TimerCompareMode::CLEAR);
    _timerPWM.setCompareMode(TimerCompare::B, TimerCompareMode::CLEAR);
    _timerPWM.setWaveMode(TimerWaveMode::PWM_PHASE_CORRECT);
    _leftWheel = Wheel(dirLeftPin, Side::LEFT, &_timerPWM);
    _rightWheel = Wheel(dirRightPin, Side::RIGHT, &_timerPWM);
    _timerPWM.start();
}

Navigation::~Navigation()
{
    _timerPWM.stop();
}

void Navigation::controlledTurn(Side turn, Orientation orientation, float speed, uint8_t turnStrength)
{

    float otherSpeed = speed * (1.f - ((float)(turnStrength) / 255.f));
    // Set roue princiaple
    if (turn == Side::LEFT)
    {
        _rightWheel.setSpeed(orientation, speed);
        _leftWheel.setSpeed(orientation, otherSpeed);
    }
    else
    {
        _leftWheel.setSpeed(orientation, speed);
        _rightWheel.setSpeed(orientation, otherSpeed);
    }
}

void Navigation::moveStraight(Orientation orientation, float speed)
{
    _leftWheel.setSpeed(orientation, speed);
    _rightWheel.setSpeed(orientation, speed);
}

void Navigation::stop()
{
    moveStraight(Orientation::FORWARD, 0);
}
